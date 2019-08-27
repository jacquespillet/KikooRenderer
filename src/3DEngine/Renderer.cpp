#include "Renderer.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {
namespace CoreEngine {

Renderer::Renderer(Scene* scene) {
    this->scene = scene;
}

void Renderer::Render() {
}

ForwardRenderer::ForwardRenderer(Scene* scene) : Renderer(scene) {

}

void ForwardRenderer::Render() {
    GETGL
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
    
    ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); 
    ogl->glStencilMask(0xFF); 


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

//Create custom shader for reading the tex and render on screen space quad

HDRRenderer::HDRRenderer(Scene* scene) : Renderer(scene) {
    alternateFBO = new Framebuffer;

    quad = GetQuad(scene, "plane", glm::dvec3(0), glm::dvec3(0), glm::dvec3(5), glm::dvec4(1, 1, 1, 1));
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