#include "HDRRenderer.hpp"
#include "../Scene.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/LightComponent.hpp"
#include "../BaseObjects.hpp"

#include "../PostProcessing/PostProcess.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core


namespace KikooRenderer {
namespace CoreEngine {

HDRRenderer::HDRRenderer(Scene* scene) : Renderer(scene) {
    GETGL
    SetFramebuffers();

    quadShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;

    //outputs
    out vec2 fragmentUv;
    //main
    void main()
    {
        fragmentUv = vec2(uv.x, uv.y);
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
    }
    )";

    quadShader.fragSrc = R"(
    //inputs
    #version 440
    in vec2 fragmentUv;

    //output
    layout(location = 0) out vec4 outputColor; 
    uniform sampler2D albedoTexture;
    uniform float exposure;
    uniform int isGammaCorrection;

    //main
    void main()
    {   
        if(isGammaCorrection > 0) {
            const float gamma = 2.2;
            vec3 hdrColor = texture(albedoTexture, fragmentUv).rgb;
        
            vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
            mapped = pow(mapped, vec3(1.0 / gamma));
        
            outputColor = vec4(mapped, 1.0);
        } else {
            outputColor = vec4(texture(albedoTexture, fragmentUv).rgb, 1);
        }

    }
    )";
    quadShader.name = "quad";
    quadShader.Compile();

    exposure = 1.0;

    quad = GetQuad(scene, "plane", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
    quadMaterial = quad->GetComponent<MaterialComponent>();
    quadMaterial->SetShader(quadShader);
    quad->Enable();
    
    dummyQuad = GetMiniQuad(scene, "DUmmy", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
    MaterialComponent* dummyMaterial = dummyQuad->GetComponent<MaterialComponent>();
    dummyMaterial->SetShader(quadShader);
    dummyQuad->Enable();

}

void HDRRenderer::Resize(int w, int h) {
    delete quadFBO;
    delete alternateFBO;
    delete finalFBO;

    SetFramebuffers();

    postProcessor.OnResize(w, h);
}

void HDRRenderer::AddPostEffect(PostProcess* postProcess) {
    postProcessor.AddProcess(postProcess);
    scene->triggerRefresh = true;
}
void HDRRenderer::RemovePostEffect(PostProcess* postProcess) {
    postProcessor.RemoveProcess(postProcess);
    scene->triggerRefresh = true;
}


void HDRRenderer::SetFramebuffers() {
    //FBO that will be rendered on the quad : Should not be multisampled
    quadFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    
    //Alternate FBO used for rendering multisampled when using MSAA
    if(useMSAA){alternateFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, true);}
    
    //Final FBO if post processing
    finalFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    
}
 
void HDRRenderer::SetGammaCorrection(bool value) {
    scene->glWindow->makeCurrent();
    GETGL
    gammaCorrection = value;
    ogl->glUseProgram(quadMaterial->shader.programShaderObject);
    ogl->glUniform1i(ogl->glGetUniformLocation(quadMaterial->shader.programShaderObject, "isGammaCorrection"), gammaCorrection);	
    scene->glWindow->doneCurrent();
    scene->triggerRefresh = true;
}


void HDRRenderer::SetMSAA(bool value) {
    useMSAA = value;

    if(!useMSAA) {
        delete alternateFBO;
    } else {
        alternateFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, true);
    }
    quadFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    scene->triggerRefresh = true;
}

void HDRRenderer::Destroy() {
    delete alternateFBO;
    delete quadFBO;
    delete finalFBO;
}

void HDRRenderer::Render() {
    GETGL   
    //Render all shadow maps
    LightComponent* light;
    TransformComponent* lightTransform;
    for(int i=0; i<scene->lightObjects.size(); i++) {
        light = scene->lightObjects[i]->GetComponent<LightComponent>();
        light->RenderDepthMap();
    }

    ogl->glViewport(0, 0, quadFBO->width, quadFBO->height);

    
    if(useMSAA) alternateFBO->Enable();
    else quadFBO->Enable();
    
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
    
    ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    ogl->glStencilMask(0xFF); 

    //Main pass
    for(int i=0; i<scene->objects3D.size(); i++) {
        if(scene->objects3D[i] && scene->objects3D[i]->visible && scene->objects3D[i]->MatchesMask(scene->GetLayerMask())) {
            scene->objects3D[i]->Render();
        }
    }

    //Set all previously updated lights to false
    for(int i=0; i<scene->lightObjects.size(); i++) {
        if(scene->lightObjects[i]->GetComponent<LightComponent>()->hasChanged) scene->lightObjects[i]->GetComponent<LightComponent>()->hasChanged = false;
        if(scene->lightObjects[i]->transform->hasChanged) scene->lightObjects[i]->transform->hasChanged = false;
    }

    for(int i=0; i<scene->objects3D.size(); i++) {
        MaterialComponent* mat = scene->objects3D[i]->GetComponent<MaterialComponent>();
        if(mat) {
            if(mat->firstIter) mat->firstIter = false;

        }
    }
    
    //Render skybox
    if(scene->hasSkybox) {
        ogl->glDepthFunc(GL_LEQUAL);        
        scene->skyboxCube->Render();
        ogl->glDepthFunc(GL_LESS);
    }

    //Render UI
    if(scene->rendersUI) {
        if(scene->renderGrid) scene->grid->Render();
        if(scene->renderAxes) scene->axes->Render();
        if(scene->renderAxesHelper) scene->axesHelper->Render();
        if (scene->renderTransformWidget && scene->transformWidget->visible && scene->selectedObjects.size() > 0 && scene->selectedObjects[0]->visible) {
            scene->transformWidget->Render();
        }
             
    }
    
    //Late render of objects
    for(int i=0; i<scene->objects3D.size(); i++) {
        if(scene->objects3D[i] && scene->objects3D[i]->visible && scene->objects3D[i]->MatchesMask(scene->GetLayerMask())) {
            scene->objects3D[i]->LateRender();
        }
    }    

    if(useMSAA) alternateFBO->Disable();
    else quadFBO->Disable();

    //If rendered in multisampled framebuffer, blit it into single sample framebuffer
    if(useMSAA){
        alternateFBO->CopyToFramebuffer(quadFBO);
    }
    
    //If post processes, process, else render the FBO on the quad
    if(postProcessor.numProcesses >0) {
        postProcessor.Run(quadFBO, finalFBO);
        finalFBO->RenderFBOToObject(quad);
    } else {	
        quadFBO->RenderFBOToObject(quad, false);
    }
}
}
} 