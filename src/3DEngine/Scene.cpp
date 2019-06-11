#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {

namespace CoreEngine {
    Scene::Scene() : camera(CameraScene(this, 1.0, 70 * DEGTORAD, 0.1, 1000.0, 1.0)){
        this->started = false;
    }

    void Scene::Start() {
        this->started = true;
        OnStart();


        //ADD OBJECTS HERE 
        
        // Object3D* newObject = GetQuad(this);
        // Object3D* sphere = GetSphere(this);
        // objects3D.push_back(sphere);

        Object3D* plane = GetGrid(this);
        objects3D.push_back(plane);

        //Start each object
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Start();
        }        
    }

    void Scene::Enable() {
        standardShaders.Compile();
        //Enable each Object3D in scene
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Enable();
        }
    }

    void Scene::Render() {
        GETGL
        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render each object
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Render(); 
        }
    }

    void Scene::OnUpdate() {
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Update();            
        }
    }

    //AddObject

    //RemoveObject

    //FindObjectByName

    //Enable

    //Disable

    void Scene::OnDestroy() { 
        std::cout << "Destroying scene" << std::endl;
    }


    void Scene::OnKeyPressEvent(QKeyEvent *e){
        this->camera.OnKeyPressEvent(e);        
    }

    void Scene::SetWindowSize(int w, int h) {
        this->windowWidth = w;
        this->windowHeight = h;
        float aspectRatio = w/h;

        this->camera.aspect = aspectRatio;
        this->camera.UpdateProjectionMatrix();
    }
}
}