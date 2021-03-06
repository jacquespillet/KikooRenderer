#include "Renderer.hpp"
#include "../Scene.hpp"
#include "../Components/MaterialComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {
namespace CoreEngine {

Renderer::Renderer(Scene* scene) {
    this->scene = scene;
}

void Renderer::Render() {}
void Renderer::Resize(int w, int h) {}
void Renderer::SetMSAA(bool value) {}
void Renderer::Destroy() {}
void Renderer::AddPostEffect(PostProcess* postProcess) {}
void Renderer::RemovePostEffect(PostProcess* postProcess) {}

void Renderer::SetGammaCorrection(bool value) {
    this->scene->glWindow->makeCurrent();
    GETGL
    if(value) ogl->glEnable(GL_FRAMEBUFFER_SRGB); 
    else ogl->glDisable(GL_FRAMEBUFFER_SRGB); 
    this->scene->glWindow->doneCurrent();
    scene->triggerRefresh = true;

    isGammaCorrected = value;
}

void ForwardRenderer::Destroy() {}

ForwardRenderer::ForwardRenderer(Scene* scene) : Renderer(scene) {
}

void ForwardRenderer::SetMSAA(bool value) {
    GETGL
    if(value) {
        ogl->glEnable(GL_MULTISAMPLE);  
    } else {
        ogl->glDisable(GL_MULTISAMPLE);  
    }
    scene->triggerRefresh = true;
}

void ForwardRenderer::AddPostEffect(PostProcess* postProcess) {}
void ForwardRenderer::RemovePostEffect(PostProcess* postProcess) {}

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
        if(scene->renderGrid) scene->grid->Render();
        if(scene->renderAxes) scene->axes->Render();
        if(scene->renderAxesHelper) scene->axesHelper->Render();
        if (scene->renderTransformWidget && scene->transformWidget->visible && scene->selectedObjects.size() > 0 && scene->selectedObjects[0]->visible) {
            scene->transformWidget->Render();
        }

        ogl->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        ogl->glStencilMask(0x00); 
        ogl->glDisable(GL_DEPTH_TEST);

        //Render selected objects
        for(int i=0; i<scene->selectedObjects.size(); i++) {
            if(scene->selectedObjects[i]->visible) {
                MaterialComponent* material = (scene->selectedObjects[i]->GetComponent<MaterialComponent>());
                if(material) {
                    //Save shader state to set it back after this pass
                    material->shader.shouldRecompile = false;
                    Shader tmpShader = material->shader;
                    ShaderParams* tmpParams = material->params;

                    material->SetShader(scene->standardShaders.selectedObjectShader);
                    scene->selectedObjects[i]->Render();
                    material->SetShader(tmpShader);
                    material->params = tmpParams;
                    material->shader.shouldRecompile = true;
                }
            }
        }

        ogl->glStencilMask(0xFF);
        ogl->glEnable(GL_DEPTH_TEST);
    }
}

}
}