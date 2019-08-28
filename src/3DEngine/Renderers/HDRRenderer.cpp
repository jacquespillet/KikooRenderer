#include "HDRRenderer.hpp"
#include "../Scene.hpp"
#include "../Components/MaterialComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {
namespace CoreEngine {

HDRRenderer::HDRRenderer(Scene* scene) : Renderer(scene) {
    alternateFBO = new Framebuffer;

    quadShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;
    //transforms
    uniform mat4 modelViewProjectionMatrix;
    uniform vec4 albedo; 
    uniform float materialInfluence;
    uniform int hasCubemap;

    //outputs
    out vec4 fragmentColor;  
    out vec2 fragmentUv;
    out vec3 cubeTexCoords;
    //main
    void main()
    {
        fragmentColor = materialInfluence * albedo + (1.0 - materialInfluence) * color;

        vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);

        fragmentUv = uv;
        cubeTexCoords = position.xyz;

        gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
        if(hasCubemap > 0) gl_Position = gl_Position.xyww; 
    }
    )";

    quadShader.fragSrc = R"(
    //inputs
    #version 440
    in vec4 fragmentColor; 
    in vec2 fragmentUv;
    in vec3 cubeTexCoords;
    //uniforms
    uniform int hasAlbedoTex;
    uniform sampler2D albedoTexture;
    uniform samplerCube cubemapTexture;

    uniform int hasCubemap;
    //output
    layout(location = 0) out vec4 outputColor; 
    //main
    void main()
    {
        if(hasCubemap > 0) {
            outputColor = texture(cubemapTexture, cubeTexCoords);
        } else {
            outputColor = (hasAlbedoTex==1) ? texture(albedoTexture, fragmentUv) : fragmentColor;
        }
    }
    )";
    quadShader.name = "quad";
    std::cout << "StandardShaders: Compiling quadShader" << std::endl; 
    quadShader.Compile();

    quad = GetQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(5), glm::dvec4(1, 1, 1, 1));
    MaterialComponent* material = (MaterialComponent*) quad->GetComponent("Material");
    material
    quad->Enable();
}

void HDRRenderer::Render() {
    GETGL
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
    
    ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    ogl->glStencilMask(0xFF); 

    alternateFBO->RenderOnObect(scene->objects3D, quad);
    
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

        ogl->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        ogl->glStencilMask(0x00); 
        ogl->glDisable(GL_DEPTH_TEST);

        //Render selected objects
        for(int i=0; i<scene->selectedObjects.size(); i++) {
            if(scene->selectedObjects[i]->visible) {
                MaterialComponent* material = (MaterialComponent*)(scene->selectedObjects[i]->GetComponent("Material"));
                if(material) {
                    //Save shader state to set it back after this pass
                    Shader* tmpShader = material->shader;
                    ShaderParams* tmpParams = material->params;

                    material->SetShader(&scene->standardShaders.selectedObjectShader);
                    scene->selectedObjects[i]->Render();
                    material->SetShader(tmpShader);
                    material->params = tmpParams;
                }
            }
        }

        ogl->glStencilMask(0xFF);
        ogl->glEnable(GL_DEPTH_TEST);
    }
}

}
}