#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Components/BoundingComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Geometry/Ray.hpp"
#include "Util/ModelLoader.hpp"
#include "SpatialPartitioning/Octree.hpp"
#include "Framebuffer.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {

namespace CoreEngine {
    Scene::Scene() : camera(CameraScene(this, 1.0, 70 * DEGTORAD, 0.1, 1000.0, 1.0)){
        this->started = false;
        transformOffset = 0;
        isFirstTransformFrame = false;
    }
	Framebuffer* alternateFBO;
	Object3D* quad;
	void Scene::Start() {
		GETGL

        this->started = true;
        OnStart();

        transformWidget = GetTranslateWidget(this, "TranslateWidget", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1));
        transformWidget->visible = false;
        AddObject(transformWidget);

        Object3D* grid = GetGrid(this, "Grid");
        AddObject(grid);

        Object3D* axes = GetAxes(this, "Axes");
        AddObject(axes);

        transformWidget = GetTranslateWidget(this, "TranslateWidget", glm::dvec3(0), glm::dvec3(0), glm::dvec3(1));
        transformWidget->visible = false;
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
		/*
		SpatialPartition::Octree octree;
		octree.Build(this, objects3D, 3);
		*/
    }

    void Scene::Render() {
		GETGL

        ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
        
        ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        ogl->glStencilMask(0xFF); 


		// Render each object
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i]->visible && objects3D[i] != transformWidget ) {
                objects3D[i]->Render(); 
            }
        }

        if(transformWidget->visible) transformWidget->Render();

        ogl->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        ogl->glStencilMask(0x00); 
        ogl->glDisable(GL_DEPTH_TEST);

        //Render selected objects
        for(int i=0; i<selectedObjects.size(); i++) {
            if(selectedObjects[i]->visible) {
                MaterialComponent* material = (MaterialComponent*)(selectedObjects[i]->GetComponent("Material"));
                if(material) {
                    Shader* tmpShader = material->shader;
                    material->SetShader(&standardShaders.selectedObjectShader);
                    selectedObjects[i]->Render();
                    material->SetShader(tmpShader);
                }
            }
        }
        
        ogl->glStencilMask(0xFF);
        ogl->glEnable(GL_DEPTH_TEST); 
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
        bool nameIsOk = (objects3D.size() == 0);
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

		alternateFBO->Destroy();
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

        if(isTransforming) { 
            isTransforming = false;
            isFirstTransformFrame = false;
            transformOffset = 0;
        }
    }

    void Scene::OnMouseMoveEvent(QMouseEvent *e) {
        this->camera.OnMouseMoveEvent(e);  
        
        if(isTransforming) TransformSelection(e);
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
                isTransforming = true;
                isFirstTransformFrame = true;
                if(intersectedObject->name == "coneX" || intersectedObject->name == "cubeX") transformAxis = TRANSFORM_AXIS::X;
                else if(intersectedObject->name == "coneY" || intersectedObject->name == "cubeY") transformAxis = TRANSFORM_AXIS::Y;
                else if(intersectedObject->name == "coneZ" || intersectedObject->name == "cubeZ") transformAxis = TRANSFORM_AXIS::Z;
            } else {
				AddObjectToSelection(true, intersectedObject);

                //MULTIPLE SELECTION : add CTRL + CLICK selection
                // intersectedObject->isSelected = !intersectedObject->isSelected;
                // std::vector<Object3D*>::iterator it = std::find(selectedObjects.begin(), selectedObjects.end(), intersectedObject);
                // int objectInx = std::distance(selectedObjects.begin(), it); 
                
                // if( it != selectedObjects.end()) {
                //     selectedObjects.erase(selectedObjects.begin() + objectInx);
                // } else {
                //     selectedObjects.push_back(intersectedObject);
                // }

                // TransformComponent* objectTransform = (TransformComponent*) intersectedObject->GetComponent("Transform");
                // TransformComponent* widgetTransform = (TransformComponent*) transformWidget->GetComponent("Transform");
                // widgetTransform->position = objectTransform->position;
                
                // transformWidget->visible = selectedObjects.size() > 0;
            }
        } else {
            selectedObjects.resize(0);
            transformWidget->visible = false;
        } 
    }

	void Scene::AddObjectToSelection(bool erasePrevious, Object3D* intersectedObject) {
		
		if(erasePrevious) selectedObjects.resize(0);
	
		intersectedObject->isSelected = !intersectedObject->isSelected;

		std::vector<Object3D*>::iterator it = std::find(selectedObjects.begin(), selectedObjects.end(), intersectedObject);
		int objectInx = std::distance(selectedObjects.begin(), it);
		if (it != selectedObjects.end()) {
			selectedObjects.erase(selectedObjects.begin() + objectInx);
		}
		else {
			selectedObjects.push_back(intersectedObject);
		}

		TransformComponent* objectTransform = intersectedObject->transform;
		TransformComponent* widgetTransform = transformWidget->transform;
		widgetTransform->position = objectTransform->position;

		transformWidget->visible = selectedObjects.size() > 0;
	}

	void Scene::ClearSelection() {
		selectedObjects.resize(0);
		transformWidget->visible = false;
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

    void Scene::TransformSelection(QMouseEvent *e) {
        int newX = e->x();
        int newY = e->y();

        Geometry::Ray ray = this->camera.GetRay(newX, newY);
        double dotX = std::abs(glm::dot(ray.direction, glm::dvec3(1, 0, 0)));
        double dotY = std::abs(glm::dot(ray.direction, glm::dvec3(0, 1, 0)));
        double dotZ = std::abs(glm::dot(ray.direction, glm::dvec3(0, 0, 1)));
        
        for(int i=0; i<selectedObjects.size(); i++) {
            TransformComponent* objectTransform = selectedObjects[i]->transform; 
            TransformComponent* widgetTransform = transformWidget->transform; 
            if(objectTransform != nullptr) {
                if(transformMode == TRANSFORM_MODE::TRANSLATE) {
                    if(transformAxis == TRANSFORM_AXIS::X) {
                        double ysign = widgetTransform->position.y > 0 ? -1 : 1;
                        double zsign =  widgetTransform->position.z > 0 ? -1 : 1;

                        glm::dvec4 plane = (dotY > dotZ) ? glm::dvec4(0, ysign * 1, 0, std::abs(widgetTransform->position.y)) : glm::dvec4(0, 0, zsign * 1, std::abs(widgetTransform->position.z));
                        double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
                        
                        glm::dvec3 position = ray.origin + t * ray.direction;
                        if(isFirstTransformFrame) { //First frame of transformation, get offset btw click and origin of transform widget
                            isFirstTransformFrame = false;
                            //Get position of ray on the plane
                            transformOffset = position.x - widgetTransform->position.x;
                        } else {
                            //Set transform to x position of intersection
                            double newX = position.x - transformOffset;
                            widgetTransform->position.x = newX;
                            objectTransform->position.x= newX;
                        }
                    } else if(transformAxis == TRANSFORM_AXIS::Y) {
                        double xsign = widgetTransform->position.x > 0 ? -1 : 1;
                        double zsign = widgetTransform->position.z > 0 ? -1 : 1;

                        glm::dvec4 plane = (dotX > dotZ) ? glm::dvec4(xsign * 1, 0, 0, std::abs(widgetTransform->position.x)) : glm::dvec4(0, 0,zsign *  1, std::abs(widgetTransform->position.z));
                        
                        double t= -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
                        glm::dvec3 position = ray.origin + t * ray.direction;
                        
                        if(isFirstTransformFrame) { 
                            isFirstTransformFrame = false;
                            transformOffset = position.y - widgetTransform->position.y;
                        } else {                                
                            double newY = position.y - transformOffset;
                            widgetTransform->position.y = newY;
                            objectTransform->position.y = newY;
                        }
                    } else if(transformAxis == TRANSFORM_AXIS::Z) {
                        double xsign = widgetTransform->position.x > 0 ? -1 : 1;
                        double ysign = widgetTransform->position.y > 0 ? -1 : 1;
                        glm::dvec4 plane = (dotX > dotY) ? glm::dvec4(xsign * 1, 0, 0, std::abs(widgetTransform->position.x)) : glm::dvec4(0, ysign * 1, 0, std::abs(widgetTransform->position.y));
                        
                        double t= -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
                        glm::dvec3 position = ray.origin + t * ray.direction;
                        
                        if(isFirstTransformFrame) { 
                            isFirstTransformFrame = false;
                            
                            transformOffset = position.z - widgetTransform->position.z;
                        } else {                            
                            double newZ = position.z - transformOffset;
                            widgetTransform->position.z = newZ;
                            objectTransform->position.z = newZ;
                        }                        
                    }
                }
            }
        }
    }
}
}
