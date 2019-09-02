#include "HDRRenderer.hpp"
#include "../Scene.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/LightComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {
namespace CoreEngine {

HDRRenderer::HDRRenderer(Scene* scene) : Renderer(scene) {
    GETGL

    alternateFBO = new Framebuffer(scene->windowWidth, scene->windowHeight, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true);
    

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
        fragmentUv = vec2(uv.x, -uv.y);
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

    //main
    void main()
    {   
        const float gamma = 2.2;
        vec3 hdrColor = texture(albedoTexture, fragmentUv).rgb;
    
        vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
        mapped = pow(mapped, vec3(1.0 / gamma));
    
        outputColor = vec4(mapped, 1.0);

    }
    )";
    quadShader.name = "quad";
    std::cout << "StandardShaders: Compiling quadShader" << std::endl; 
    quadShader.Compile();

    exposure = 1.0;

    quad = GetQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
    MaterialComponent* material = (MaterialComponent*) quad->GetComponent("Material");
    material->SetShader(&quadShader);
    
    dummyQuad = GetMiniQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
    MaterialComponent* dummyMaterial = (MaterialComponent*) dummyQuad->GetComponent("Material");
    dummyMaterial->SetShader(&quadShader);
    
    // int exposureLocation = ogl->glGetUniformLocation(quadShader.programShaderObject, "exposure"); 
    // ogl->glUniform1f(exposureLocation, exposure);
    dummyQuad->Enable();
    quad->Enable();
}

void HDRRenderer::Resize(int w, int h) {
    delete alternateFBO;
    alternateFBO = new Framebuffer(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true);
}

void HDRRenderer::Render() {
    GETGL   
    alternateFBO->Enable();
    
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
    
    ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    ogl->glStencilMask(0xFF); 

    //Main pass
    for(int i=0; i<scene->objects3D.size(); i++) {
        if(scene->objects3D[i] && scene->objects3D[i]->visible ) {
            scene->objects3D[i]->Render();
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
        scene->grid->Render();
        scene->axes->Render();

        if (scene->transformWidget->visible && scene->selectedObjects.size() > 0 && scene->selectedObjects[0]->visible) {
            scene->transformWidget->Render();
        }
    }

    alternateFBO->Disable();

    ogl->glCullFace(GL_FRONT);
    LightComponent* light;
    for(int i=0; i<scene->lightObjects.size(); i++) {
        light = (LightComponent*) scene->lightObjects[i]->GetComponent("Light");
        light->RenderDepthMap();
    }
    ogl->glCullFace(GL_BACK);

    
    ogl->glViewport(0, 0, alternateFBO->width, alternateFBO->height);
    alternateFBO->RenderFBOToObject(quad);
    // light->depthFBO->RenderFBOToObject(dummyQuad, true);

}

}
} 