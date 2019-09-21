#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Geometry/Ray.hpp"
#include "Framebuffer.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/LightComponent.hpp"

#include "ParticleSystem/ParticleSystem.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
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
        standardShaders.Compile();

        this->renderer = new HDRRenderer(this);
        // this->renderer = new ForwardRenderer(this);

        grid = GetGrid(this, "Grid");
        grid->Start();
        grid->Enable();

        axes = GetAxes(this, "Axes");
        axes->Start();
        axes->Enable();

		transformWidget = new TransformWidget(this);
        transformWidget->Start();
		transformWidget->Enable();

        skyboxCube = GetCube(this, "Cubemap", glm::dvec3(0), glm::vec3(0), glm::dvec3(100), glm::dvec4(0.1, 0.1, 0.1, 1));
        skyboxCube->Start();
        skyboxCube->Enable();

        ParticleSystem* ps = new ParticleSystem("ps", this);
        AddObject(ps);

        //Start each object
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Start();
        }
    }

    void Scene::Enable() {
        for(int i=0; i<objects3D.size(); i++) {
            if(!objects3D[i]->started) objects3D[i]->Start(); 
            objects3D[i]->Enable();
        }
    }

    void Scene::Render() {
		GETGL
        this->renderer->Render();
    }

    void Scene::OnUpdate() {
        for(int i=0; i<objects3D.size(); i++) {
            if(!objects3D[i]->started) objects3D[i]->Start(); 
            if(!objects3D[i]->enabled) objects3D[i]->Enable();
            objects3D[i]->Update();
        }
        if(hasSkybox) {
            skyboxCube->Update();
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
        LightComponent* light = (LightComponent*)object->GetComponent("Light");
		if (light != nullptr) {
			for (int i = 0; i < lightObjects.size(); i++) {
				if (lightObjects[i] == object) {
					lightObjects.erase(lightObjects.begin() + i);
					break;
				}
			}
		}

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
    }


    //Disable

    void Scene::OnDestroy() { 
        std::cout << "Destroying scene" << std::endl;
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->Destroy();
            delete objects3D[i];
        }
        renderer->Destroy();
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

        if (e->key() == Qt::Key_Delete) {
            DeleteSelection();
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
        renderer->Resize(w, h);
    }
    void Scene::SetRenderPipeline(RENDER_PIPELINE pipeline) {
        this->glWindow->makeCurrent();
        switch (pipeline)
        {
            case RENDER_PIPELINE::FORWARD:
                this->renderer = new ForwardRenderer(this);
                break;
            case RENDER_PIPELINE::HDR:
                this->renderer = new HDRRenderer(this);
                break;
            default:
                break;
        }
        this->glWindow->doneCurrent();
        this->triggerRefresh = true;
    }

    void Scene::HandleSelection(int x, int y) {
        Object3D* intersectedObject = GetIntersectObject(x, y);
        if(intersectedObject != nullptr) {
			std::cout << intersectedObject->name << std::endl;
			AddObjectToSelection(true, intersectedObject);
        } else if(!transformWidget->visible) { 
            ClearSelection();
		}
    }

	void Scene::AddObjectToSelection(bool erasePrevious, Object3D* intersectedObject) {
		
		if(erasePrevious) selectedObjects.resize(0);
	
		intersectedObject->isSelected = !intersectedObject->isSelected;

        //Check if it was in the selectedObjects array. If yes, remove it. If no, add it to selection
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

    void Scene::DeleteSelection() {
        for(int i=0; i<selectedObjects.size(); i++) {
            objectDetailsPanel->sceneTree->DeleteObject(selectedObjects[i]);
        }
        ClearSelection();
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

    // Called from preferences, has no GL context
    void Scene::SetSkybox(std::vector<std::string> filenames) {
        glWindow->makeCurrent();
        MaterialComponent* material = (MaterialComponent*) skyboxCube->GetComponent("Material");
        material->SetCubemap(filenames);
        hasSkybox = true;
        glWindow->doneCurrent();
        triggerRefresh = true;
    }

    QJsonObject Scene::ToJSON() {
        QJsonObject json;
        QJsonArray objectArray;
        for(int i=0; i<objects3D.size(); i++) {
            QJsonObject jsonObject = objects3D[i]->ToJSON();
            objectArray.append(jsonObject);
        }
        json["objects"] = objectArray;        
        return json;
    }
}
}
