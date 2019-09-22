#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Geometry/Ray.hpp"
#include "Framebuffer.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/LightComponent.hpp"

#include "ParticleSystem/ParticleSystem.hpp"

#include "WaterTiles/WaterTile_1.hpp"

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

        WaterTile_1* wt1 = new WaterTile_1("wt1", this);
        wt1->Start();
        wt1->Enable();
        AddObject(wt1);

        Object3D* cube0 = GetCube(this, "cube0", glm::dvec3(0), glm::vec3(0), glm::dvec3(30), glm::dvec4(1));
        MaterialComponent* mat = (MaterialComponent*) cube0->GetComponent("Material");
        mat->flipNormals = true;
        mat->albedoTex = Texture("C:/Users/Jacques/Pictures/Textures/Grid/Glass_Pattern_002_basecolor.jpg", GL_TEXTURE0); 
        cube0->Start();
        cube0->Enable();
        AddObject(cube0);

        Object3D* cube1 = GetCube(this, "cube1", glm::dvec3(-8, 1, 1), glm::vec3(0), glm::dvec3(2, 2, 2), glm::dvec4(1, 0.2, 0.1, 1));
        cube1->Start();
        cube1->Enable();
        AddObject(cube1);

        
        Object3D* cube2 = GetCube(this, "cube2", glm::dvec3(8, 1, -1), glm::vec3(0), glm::dvec3(3, 2, 2), glm::dvec4(1, 0.2, 0.1, 1));
        cube2->Start();
        cube2->Enable();
        AddObject(cube2);

        
        Object3D* cube3 = GetCube(this, "cube3", glm::dvec3(0, 1, 8), glm::vec3(0), glm::dvec3(2, 1, 4), glm::dvec4(1, 0.2, 0.1, 1));
        cube3->Start();
        cube3->Enable();
        AddObject(cube3);

        
        Object3D* cube4 = GetCube(this, "cube4", glm::dvec3(-3, -2, 3), glm::vec3(0), glm::dvec3(4, 1, 2), glm::dvec4(0.2, 0.8, 0.1, 1));
        cube4->Start();
        cube4->Enable();
        AddObject(cube4);

        
        Object3D* cube5 = GetCube(this, "cube5", glm::dvec3(5, -4, -4), glm::vec3(0), glm::dvec3(2, 4, 2), glm::dvec4(0.2, 0.8, 0.1, 1));
        cube5->Start();
        cube5->Enable();
        AddObject(cube5);

        
        Object3D* cube6 = GetCube(this, "cube6", glm::dvec3(0, -2, 8), glm::vec3(0), glm::dvec3(4, 1, 3), glm::dvec4(0.2, 0.8, 0.1, 1));
        cube6->Start();
        cube6->Enable();
        AddObject(cube6);




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
        for(int i=0; i<objects3D.size(); i++) {
            objects3D[i]->WindowResize(w, h);
        }
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
