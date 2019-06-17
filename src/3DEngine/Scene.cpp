#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Components/BoundingComponent.hpp"
#include "Geometry/Ray.hpp"

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

        Object3D* plane = GetGrid(this, "Grid");
        objects3D.push_back(plane);

        Object3D* axes = GetAxes(this, "Axes");
        objects3D.push_back(axes);
        
        // Object3D* widget = GetScaleWidget(this, glm::dvec3(0), glm::dvec3(0), glm::dvec3(1));
        // objects3D.push_back(widget);

        //ADD OBJECTS HERE 
        Object3D* sphere = GetSphere(this,"Sphere", glm::dvec3(1), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.8, 0.4, 0.4, 1.0));
        AddObject(sphere);

        
        Object3D* sphere1 = GetSphere(this,"Sphere", glm::dvec3(-1), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.4, 0.1, 0.1, 1.0));
        AddObject(sphere1);
        
        Object3D* sphere2 = GetSphere(this,"Sphere", glm::dvec3(1, -1, 0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.4, 0.1, 0.1, 1.0));
        AddObject(sphere2);
        // objects3D.push_back(sphere); 


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

        // Object3D* sphere = FindObjectByName("Sphere");
        // BoundingBoxComponent* bb = (BoundingBoxComponent*) sphere->GetComponent("BoundingBox");
        // Object3D* bbObj = bb->GetBoxObject();
        // bbObj->Start();
        // bbObj->Enable();
        // sphere->AddObject(bbObj);        
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


    Object3D* Scene::FindObjectByName(std::string name) {
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i]->name == name) {
                return objects3D[i];
            }
        }        
        return nullptr;
    }

    void Scene::AddObject(Object3D* object) {
        bool nameIsOk = false;
        std::string currentName = object->name;
        
        while(!nameIsOk) {
            for(int i=0; i<objects3D.size(); i++) {
                std::string otherName=objects3D[i]->name;
                if(otherName == currentName) {
                    currentName = currentName + " (1)";
                } else {
                    nameIsOk = true;
                }
            }
        }

        object->name = currentName;
        objects3D.push_back(object);
    }

    //RemoveObject
    void Scene::RemoveObject(Object3D* object) {
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i] == object) {
                delete objects3D[i];
                objects3D.erase(objects3D.begin() + i);
                break;
            }
        }
    }


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

        Object3D* intersectedObject = GetIntersectObject(e->x(), e->y());             
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

    Object3D* Scene::GetIntersectObject(int x, int y) {
        std::cout << x << std::endl;
        std::cout << y << std::endl;
        Geometry::Ray ray = this->camera.GetRay(x, y);
        // glm::dvec4 p1 = glm::dvec4()

        return nullptr;
    }

}
}