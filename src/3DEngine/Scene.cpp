#include "Scene.hpp"
#include "Util.hpp"
#include "BaseObjects.hpp"
#include "Geometry/Ray.hpp"
#include "Framebuffer.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Components/BoundingComponent.hpp"

#include "Shaders/BlinnPhongShader.hpp"

#include "Util/RandomUtil.hpp"

#include "3DEngine/Text/TexturedText/TexturedText.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
    Scene::Scene() : drawImmediate(this) {
		camera = new CameraScene(this, 1.0, 70 * DEGTORAD, 0.0001, 1000.0, 1.0);
        this->started = false;
    }
    
	void Scene::Start() {

		GETGL
        this->started = true;
        standardShaders.Compile();

        this->renderer = new DeferredRenderer(this);
        // this->renderer = new HDRRenderer(this);
        // this->renderer = new ForwardRenderer(this);

        grid = GetGrid(this, "Grid");
        grid->Start();
        grid->Enable();

        axes = GetAxes(this, "Axes");
        axes->Start();
        axes->Enable();

        axesHelper = GetAxesHelper(this);
        axesHelper->Start();
        axesHelper->Enable();

		transformWidget = new TransformWidget(this);
        transformWidget->Start();
		transformWidget->Enable();

        skyboxCube = GetCube(this, "Cubemap", glm::vec3(0), glm::vec3(0), glm::vec3(100), glm::vec4(0.1, 0.1, 0.1, 1));
        skyboxCube->GetComponent<MaterialComponent>()->SetShader(standardShaders.unlitMeshShader);
        skyboxCube->Start();
        skyboxCube->Enable();

        simulation.SetScene(this);
        simulation.worldType = KikooRenderer::Physics::Bullet::WORLD_TYPE::RIGID;
        simulation.Init();
        
        drawImmediate.Init();

        TexturedText* text = new TexturedText("test", this);
        text->SetText("qfewoikqwjfoiewj");
        AddObject(text);

        Object3D* cube = GetCube(this, "cub", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1));
        AddObject(cube);

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
        if(isPlaying) {
            animationFrametime+= deltaTime;
            simulation.Simulate();
        }

        for (int i=0; i<objects3D.size(); i++) {
            if(!objects3D[i]->started) objects3D[i]->Start(); 
            if(!objects3D[i]->enabled) objects3D[i]->Enable();
            objects3D[i]->Update();
        }
        camera->OnUpdate();
        if(hasSkybox) {
            skyboxCube->Update();
        }        
        transformWidget->Update();
        elapsedTime += deltaTime;
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

        if(!object->isRayMarched) {
            objects3D.push_back(object);
        } else {
            rayMarchedObjects.push_back(object);
        }

		LightComponent* light =  object->GetComponent<LightComponent>();
		if (light != nullptr) {
			lightObjects.push_back(object);
		}

        sceneTree->AddObject(object);
        return currentName;
    }

    //RemoveObject
    void Scene::RemoveObject(Object3D* object) {
        LightComponent* light = object->GetComponent<LightComponent>();
		if (light != nullptr) {
			for (int i = 0; i < lightObjects.size(); i++) {
				if (lightObjects[i] == object) {
					lightObjects.erase(lightObjects.begin() + i);
					break;
				}
			}
		}
        
        for (int i = 0; i < rayMarchedObjects.size(); i++) {
            if (rayMarchedObjects[i] == object) {
                rayMarchedObjects.erase(rayMarchedObjects.begin() + i);
                break;
            }
        }

        for(int i=0; i<objects3D.size(); i++) {
            if(objects3D[i] == object) {
				// for (int j = 0; j < object->childObjects.size(); j++) {
				// 	RemoveObject(object->childObjects[j]);
				// }
                delete objects3D[i];
                objects3D.erase(objects3D.begin() + i);
                break;
            }
        }

        simulation.RemoveObject(object);
    }


    //Disable

    void Scene::OnDestroy() { 
        simulation.Destroy();
        std::cout << "Scene:OnDestroy: Destroying scene" << std::endl;
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

        if(e->key() == Qt::Key_E) {
            isEditMode = !isEditMode;
            for(int i=0; i<selectedObjects.size(); i++) {
                selectedObjects[i]->ToggleEditing();
                if(isEditMode) {
                    editedObject = selectedObjects[i];
                } else {
                    editedObject = nullptr;
                }
            }
        }

        if (e->key() == Qt::Key_Delete) {
            DeleteSelection();
        }   
	}

    void Scene::OnMousePressEvent(QMouseEvent *e) {
        this->camera->OnMousePressEvent(e);
        if(e->button() == Qt::LeftButton) HandleSelection(e->x(), e->y(), (e->modifiers() & Qt::ControlModifier));
    }

    void Scene::OnMouseDoubleClickEvent( QMouseEvent * e ) {
        
    }

	void Scene::OnKeyReleaseEvent(QKeyEvent* e) {
        camera->OnKeyReleaseEvent(e);
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
        float aspectRatio = (float)w/(float)h;

        this->camera->SetAspect(aspectRatio);
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

    void Scene::GetObjectsInSquare(glm::vec2 topLeft, glm::vec2 bottomRight) {
        struct Box2D {
            int x;
            int y;
            int width;
            int height;
        };

        Box2D a;
        a.width = (bottomRight.x > topLeft.x) ? (bottomRight.x - topLeft.x) : (topLeft.x - bottomRight.x);
        a.height = (bottomRight.y > topLeft.y ) ?  (bottomRight.y - topLeft.y) : (topLeft.y - bottomRight.y);
        a.x =   (bottomRight.x + topLeft.x) * 0.5;
        a.y =   (bottomRight.y + topLeft.y) * 0.5;
        glm::mat4 viewProj = camera->GetProjectionMatrix() * camera->GetViewMatrix();
        for(int i=0; i< objects3D.size(); i++) {
            if(std::find(selectedObjects.begin(), selectedObjects.end(), objects3D[i]) == selectedObjects.end()) {
                BoundingBoxComponent* bb = objects3D[i]->GetComponent<BoundingBoxComponent>();
                if(bb != nullptr) {
                    glm::mat4 mvp = viewProj *  objects3D[i]->transform->GetModelMatrix();
                    glm::vec2 min, max;
                    bb->GetNDCBounds(min, max, mvp);
                    
                    min.x = ((min.x + 1.0) * 0.5)  * windowWidth;
                    max.x = ((max.x + 1.0) * 0.5)  * windowWidth;

                    min.y = (1.0 - ((min.y + 1.0) * 0.5)) * windowHeight;
                    max.y = (1.0 - ((max.y + 1.0) * 0.5)) * windowHeight;
                    
                    Box2D b;
                    b.width = (max.x - min.x);  
                    b.height = (min.y - max.y );
                    b.x =   (max.x + min.x) * 0.5;
                    b.y =   (max.y + min.y) * 0.5;

                    bool intersects = (std::abs(a.x - b.x) * 2 < (a.width + b.width)) && (std::abs(a.y - b.y) * 2 < (a.height + b.height));
                    if(intersects) {
                        AddObjectToSelection(false, objects3D[i]); 
                    }
                }
            }
        }
    }

    void Scene::HandleSelection(int x, int y, bool isCtrl) {
        Object3D* intersectedObject = GetIntersectObject(x, y);
        if(intersectedObject == transformWidget) { //Intersected transform widget --> transform
        }
        else if( intersectedObject != nullptr) { //Intersected object --> add it to selection
			AddObjectToSelection(!isCtrl, intersectedObject);
        } else { //Intersected nothing --> Clear selection
            ClearSelection(); 
		} 
    }

	void Scene::AddObjectToSelection(bool erasePrevious, Object3D* intersectedObject) {
        if(isEditMode && intersectedObject->GetIsVertex()) {
            transformWidget->Disable();
            transformWidget->AddHandleObject(intersectedObject);
        } else {
            if(erasePrevious) ClearSelection();
            intersectedObject->isSelected = true;

            std::vector<Object3D*>::iterator it = std::find(selectedObjects.begin(), selectedObjects.end(), intersectedObject);
            if (it == selectedObjects.end()) {
                selectedObjects.push_back(intersectedObject);
                transformWidget->AddHandleObject(intersectedObject);
                objectDetailsPanel->SetObject(intersectedObject);
            }
        }
	}

	void Scene::ClearSelection() {
        if(isEditMode) {
            transformWidget->Disable();
            objectDetailsPanel->Clear();
            
            transformWidget->AddHandleObject(editedObject);
            objectDetailsPanel->SetObject(editedObject);
        } else {
            selectedObjects.resize(0);
            transformWidget->Disable();
            objectDetailsPanel->Clear();
        }
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
        if(intersectedTransformWidget != nullptr) return transformWidget;

		if (intersectedTransformWidget == nullptr) {
            if(isEditMode) {
                for(int i=0; i<editedObject->GetEditionObjects().size(); i++) {
                    double distance;
                    Object3D* intersectedObject = editedObject->GetEditionObjects()[i]->Intersects(ray, distance);

                    if(intersectedObject != nullptr) {
                        if(distance < minDistance) {
                            minDistance = distance;
                            closest = intersectedObject;
                        }
                    }
                }
            } else {
                for(int i=0; i<objects3D.size(); i++) {
                    double distance;
                    Object3D* intersectedObject = objects3D[i]->Intersects(ray, distance);
                    std::cout << objects3D[i]->name << "  " << distance << std::endl;
                    if(intersectedObject != nullptr) {
                        if(distance < minDistance) {
                            minDistance = distance;
                            closest = intersectedObject;
                        }
                    }
                }
            }
		}
        return closest;
    }

    Physics::Bullet::Simulation* Scene::GetSimulation() {
        return &simulation;
    }

    // Called from preferences, has no GL context
    void Scene::SetSkybox(std::vector<std::string> filenames) {
        glWindow->makeCurrent();
        MaterialComponent* material =  skyboxCube->GetComponent<MaterialComponent>();
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

    void Scene::FromJSON(QJsonDocument json) {
        glWindow->makeCurrent();
        QJsonArray objectArray = json["objects"].toArray();
        for (int objectIndex = 0; objectIndex < objectArray.size(); ++objectIndex) {
            QJsonObject objectJson = objectArray[objectIndex].toObject();
            Object3D* object = Object3D::FromJSON(objectJson, this);
            AddObject(object);
        }
        glWindow->doneCurrent();
    }

    
    void Scene::SetLayerMask(uint16_t _layerMask) {
        this->layerMask = _layerMask;
    }

    uint16_t Scene::GetLayerMask() {
        return this->layerMask;
    }


    void Scene::PlayPause() {
        this->isPlaying = !this->isPlaying;
    }

    void Scene::Stop() {
        isPlaying = false;
        simulation.Stop();
    }    

    bool Scene::IsPlaying() {
        return isPlaying;
    }
}
}