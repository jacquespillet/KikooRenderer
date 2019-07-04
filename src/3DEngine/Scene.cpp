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


        Object3D* grid = GetGrid(this, "Grid");
        objects3D.push_back(grid);

        Object3D* axes = GetAxes(this, "Axes");
        objects3D.push_back(axes);
        
        transformWidget = GetTranslateWidget(this, "TranslateWidget", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1));
        transformWidget->visible = false;
        transformWidget->depthTest = false;
        AddObject(transformWidget);

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
    }

    void Scene::Render() {
        GETGL
        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render each object
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i]->visible) {
                objects3D[i]->Render(); 
            }
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

    std::string Scene::AddObject(Object3D* object) {
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

        return currentName;
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
        if(e->button() == Qt::LeftButton) HandleSelection(e->x(), e->y());
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

    void Scene::HandleSelection(int x, int y) {
        Object3D* intersectedObject = GetIntersectObject(x, y);
        if(intersectedObject != nullptr) {
            std::cout << intersectedObject->name << std::endl;
            if(intersectedObject->parent == transformWidget) {
                std::cout << "Handle transform" << std::endl;
            } else {
                intersectedObject->isSelected = !intersectedObject->isSelected;
                std::vector<Object3D*>::iterator it = std::find(selectedObjects.begin(), selectedObjects.end(), intersectedObject);
                int objectInx = std::distance(selectedObjects.begin(), it); 
                
                if( it != selectedObjects.end()) {
                    selectedObjects.erase(selectedObjects.begin() + objectInx);
                } else {
                    selectedObjects.push_back(intersectedObject);
                }

                TransformComponent* objectTransform = (TransformComponent*) intersectedObject->GetComponent("Transform");
                TransformComponent* widgetTransform = (TransformComponent*) transformWidget->GetComponent("Transform");
                widgetTransform->position = objectTransform->position;
                
                transformWidget->visible = selectedObjects.size() > 0;
            }
        } else {
            selectedObjects.resize(0);
            transformWidget->visible = false;
        } 
    }


    Object3D* Scene::GetIntersectObject(int x, int y) {
        Geometry::Ray ray = this->camera.GetRay(x, y);
        double minDistance = 99999999999999.0;
        Object3D* closest = nullptr;
        for(int i=0; i<objects3D.size(); i++) { 
            double distance;
            Object3D* intersectedObject = objects3D[i]->Intersects(ray, distance);

            if(intersectedObject != nullptr) {
                if(intersectedObject->parent == transformWidget) {
                    closest = intersectedObject;
                    break;
                }
                if(distance < minDistance) {
                    minDistance = distance;
                    closest = intersectedObject;
                }
            }
        }
        return closest;
    }

}
}