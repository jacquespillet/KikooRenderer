#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Geometry/Ray.hpp"
#include "Framebuffer.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/LightComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {

namespace CoreEngine {
    Scene::Scene(){
		camera = new CameraScene(this, 1.0, 70 * DEGTORAD, 0.1, 1000.0, 1.0);
        this->started = false;
    }

	void Scene::Start() {
		GETGL

        this->started = true;

        Object3D* grid = GetGrid(this, "Grid");
        AddObject(grid);

        Object3D* axes = GetAxes(this, "Axes");
        AddObject(axes);

		transformWidget = new TransformWidget(this);
		transformWidget->Enable();

		Object3D* dirLight = GetDirectionalLight(this, "Main light", glm::dvec3(0), glm::dvec3(40, 170, 0), glm::dvec3(1), glm::dvec4(1, 1, 1, 1));
		AddObject(dirLight);
	
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
        ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
        
        ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        ogl->glStencilMask(0xFF); 


		// Render each object
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i] && objects3D[i]->visible ) {
                objects3D[i]->Render(); 
            }
        }

		if (transformWidget->visible && selectedObjects.size() > 0 && selectedObjects[0]->visible) {
			transformWidget->Render();
		}

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

		LightComponent* light = (LightComponent*) object->GetComponent("Light");
		if (light != nullptr) {
			lightObjects.push_back(object);
		}

        return currentName;
    }

    //RemoveObject
    void Scene::RemoveObject(Object3D* object) {
        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i] == object) {
				for (int j = 0; j < object->childObjects.size(); j++) {
					RemoveObject(object->childObjects[j]);
				}
                delete objects3D[i];
                objects3D.erase(objects3D.begin() + i);
                break;
            }
        }

		LightComponent* light = (LightComponent*)object->GetComponent("Light");
		if (light != nullptr) {
			for (int i = 0; i < lightObjects.size(); i++) {
				if (objects3D[i] == object) {
					delete lightObjects[i];
					objects3D.erase(objects3D.begin() + i);
					break;
				}
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
        this->camera->OnKeyPressEvent(e);

		if (e->key() == Qt::Key_Control) {
			isControlKey = true;
		}

		if (isControlKey && e->key() == Qt::Key_S) {
			transformWidget->SetTransformMode(TransformWidget::TransformMode::SCALE);
		}

		if (isControlKey && e->key() == Qt::Key_T) {
			transformWidget->SetTransformMode(TransformWidget::TransformMode::TRANSLATE);
		}

		if (isControlKey && e->key() == Qt::Key_R) {
			transformWidget->SetTransformMode(TransformWidget::TransformMode::ROTATE);
		}

	}

    void Scene::OnMousePressEvent(QMouseEvent *e) {
        this->camera->OnMousePressEvent(e);
        if(e->button() == Qt::LeftButton) HandleSelection(e->x(), e->y());
    }
	void Scene::OnKeyReleaseEvent(QKeyEvent* e) {
		if (e->key() == Qt::Key_Control) {
			isControlKey = false;
		}
	}

    void Scene::OnMouseReleaseEvent(QMouseEvent *e) {
        this->camera->OnMouseReleaseEvent(e);  
		transformWidget->OnMouseReleaseEvent(e);
    }

    void Scene::OnMouseMoveEvent(QMouseEvent *e) {
        this->camera->OnMouseMoveEvent(e);  
        
		transformWidget->OnMouseMoveEvent(e);
    }

    void Scene::OnWheelEvent(QWheelEvent *event) {
        this->camera->OnWheelEvent(event);  
    }

    void Scene::SetWindowSize(int w, int h) {
        this->windowWidth = w;
        this->windowHeight = h;
        float aspectRatio = w/h;

        this->camera->aspect = aspectRatio;
        this->camera->UpdateProjectionMatrix();
    }

    void Scene::HandleSelection(int x, int y) {
        Object3D* intersectedObject = GetIntersectObject(x, y);
        if(intersectedObject != nullptr) {
			std::cout << intersectedObject->name << std::endl;
			AddObjectToSelection(true, intersectedObject);
        } else if(!transformWidget->visible) { 
			selectedObjects.resize(0); 
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
		transformWidget->SetObject(intersectedObject);

		objectDetailsPanel->SetObject(intersectedObject);
	}

	void Scene::ClearSelection() {
		selectedObjects.resize(0);
		transformWidget->Disable();
		objectDetailsPanel->Clear();
	}


    Object3D* Scene::GetIntersectObject(int x, int y) {
        Geometry::Ray ray = this->camera->GetRay(x, y);
        double minDistance = 99999999999999.0;
        Object3D* closest = nullptr;
		
		double distance;
		Object3D* intersectedTransformWidget = nullptr;
		if(transformWidget->visible) intersectedTransformWidget = transformWidget->Intersects(ray, distance);
		
		if (intersectedTransformWidget == nullptr) {
			transformWidget->Disable();

			for(int i=0; i<objects3D.size(); i++) {
				double distance;
				Object3D* intersectedObject = objects3D[i]->Intersects(ray, distance);

				if(intersectedObject != nullptr) {
					if(distance < minDistance) {
						minDistance = distance;
						closest = intersectedObject;
					}
				}
			}
		}
        return closest;
    }
}
}
