#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Components/BoundingComponent.hpp"

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

        Object3D* plane = GetGrid(this);
        objects3D.push_back(plane);

        Object3D* axes = GetAxes(this);
        objects3D.push_back(axes);
        
        // Object3D* widget = GetScaleWidget(this, glm::dvec3(0), glm::dvec3(0), glm::dvec3(1));
        // objects3D.push_back(widget);

        //ADD OBJECTS HERE 
        Object3D* sphere = GetSphere(this, glm::dvec3(1), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.8, 0.4, 0.4, 1.0));
        objects3D.push_back(sphere);        

        //Start each object
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Start();
        } 
    }

    void Scene::Enable() {
        standardShaders.Compile();
        for(int i=0; i<objects3D.size(); i++) {
            if(!objects3D[i]->started) objects3D[i]->Start(); 
            objects3D[i]->Enable();
        }

        Object3D* sphere = FindObjectByName("Sphere");
        BoundingBoxComponent* bb = (BoundingBoxComponent*) sphere->GetComponent("BoundingBox");
        Object3D* bbObj = bb->GetBoxObject();
        bbObj->Start();
        bbObj->Enable();
        sphere->AddObject(bbObj);        
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
            if(!objects3D[i]->started) objects3D[i]->Start(); 
            if(!objects3D[i]->enabled) objects3D[i]->Enable(); 
            objects3D[i]->Update();            
        }
    }

    //AddObject

    //RemoveObject

    Object3D* Scene::FindObjectByName(std::string name) {
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i]->name == name) {
                return objects3D[i];
            }
        }        
        return nullptr;
    }


    //Enable

    //Disable

    void Scene::OnDestroy() { 
        std::cout << "Destroying scene" << std::endl;
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Destroy();
            delete objects3D[i];
        }
    }


    void Scene::OnKeyPressEvent(QKeyEvent *e){
        this->camera.OnKeyPressEvent(e);        
    }

    void Scene::OnMousePressEvent(QMouseEvent *e) {
        this->camera.OnMousePressEvent(e);  
    }
    void Scene::OnMouseReleaseEvent(QMouseEvent *e) {
        this->camera.OnMouseReleaseEvent(e);  
    }
    void Scene::OnMouseMoveEvent(QMouseEvent *e) {
        this->camera.OnMouseMoveEvent(e);  
    }
    void Scene::OnWheelEvent(QWheelEvent *event) {
        this->camera.OnWheelEvent(event);  
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