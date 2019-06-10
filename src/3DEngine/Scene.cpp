#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {

namespace CoreEngine {
    Scene::Scene() : camera(CameraScene(this, 1.0, 70 * Util::DEGTORAD, 0.1, 1000.0, 1.0)){
        this->started = false;
    }

    void Scene::Start() {
        this->started = true;
        OnStart();

        Object3D* newObject = GetCube(this);
        objects3D.push_back(newObject);

        //Start each object
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
        ogl->glClearColor(100, 0, 0, 0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render each object
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

    //Destroy


    void Scene::OnKeyPressEvent(QKeyEvent *e){
    	//Foreach object : 
        //  OnKeyPress
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