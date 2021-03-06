#include "Object3D.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/BoundingComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/LightComponent.hpp"
#include "Util.hpp"
#include "Framebuffer.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {

namespace CoreEngine {

Object3D::Object3D(std::string _name, Scene* _scene) {
    this->name = _name;
    this->scene = _scene;

	transform = new TransformComponent(this);
	// this->AddComponent(transform);
}

Object3D::~Object3D() {
	for(int i=0; i<components.size(); i++) { 
		delete components[i];
	}

	for(int i=0; i<childObjects.size(); i++) {
		delete childObjects[i];
	}
}

void Object3D::AddComponent(Component* component) {
	if(component != nullptr) {
		for(int i=0; i<components.size(); i++) { 
			if(components[i]->name == component->name) {
				components[i] = component;
				return;
			}
		}	
		this->components.push_back(component);
	}
}

std::string Object3D::AddObject(Object3D* object) {
	object->parent = this;

	bool nameIsOk = (childObjects.size() == 0);
	std::string currentName = object->name;
	while(!nameIsOk) {
		for(int i=0; i<childObjects.size(); i++) {
			std::string otherName=childObjects[i]->name;
			if(otherName == currentName) {
				currentName = currentName + " (1)";
			} else {
				nameIsOk = true;
			}
		}
	}
	object->name = currentName;
	childObjects.push_back(object);
	
	return currentName;
}

void Object3D::ClearObjects() {
	childObjects.resize(0);
}

void Object3D::Start() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Start();
	}
	started = true;
}

void Object3D::Enable() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Enable();
	}
    for(int i=0; i<components.size(); i++) {
        components[i]->OnEnable();
    }
	enabled = true;
}

void Object3D::Update() {
	if(hasToRecompute) {
		Recompute();
	}
	transform->OnUpdate();
	for(int i=0; i<components.size(); i++) {
		components[i]->OnUpdate();
	}
	
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Update();
	}
}	

void Object3D::Recompute() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Recompute();
	}

    for(int i=0; i<components.size(); i++) {
        components[i]->Recompute();
    }
}

void Object3D::Destroy() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Destroy();
	}

    for(int i=0; i<components.size(); i++) {
        components[i]->OnDestroy();
		delete components[i];
    }
}

void Object3D::WindowResize(int w, int h) {}

std::vector<QWidget*> Object3D::GetInspectorWidgets() {
	std::vector<QWidget*> res;
	res.push_back(this->transform->GetInspector());
	for(int i=0; i<this->components.size(); i++) {
		res.push_back(this->components[i]->GetInspector());
	}

	return res;
}

void Object3D::Render(glm::mat4* overrideViewMatrix) {
	GETGL
	if(!depthTest) ogl->glDisable(GL_DEPTH_TEST);
	
	glm::mat4 mMatrix = transform->GetWorldModelMatrix();

	//Render child objects
	for(int i=0; i<childObjects.size(); i++) {
		if (!childObjects[i]->started) childObjects[i]->Start();
		if (!childObjects[i]->enabled) childObjects[i]->Enable();
		childObjects[i]->Render();
	}
	

	if(transform != nullptr) {
		glm::mat4 vMatrix = (overrideViewMatrix == nullptr) ? scene->camera->GetViewMatrix() : (*overrideViewMatrix);
		glm::mat4 pMatrix = scene->camera->GetProjectionMatrix();
		
		if(faceCamera) {	
			mMatrix[0][0] = vMatrix[0][0]; mMatrix[1][0] = vMatrix[0][1]; mMatrix[2][0] = vMatrix[0][2];
			mMatrix[0][1] = vMatrix[1][0]; mMatrix[1][1] = vMatrix[1][1]; mMatrix[2][1] = vMatrix[1][2];
			mMatrix[0][2] = vMatrix[2][0]; mMatrix[1][2] = vMatrix[2][1]; mMatrix[2][2] = vMatrix[2][2];
			mMatrix = glm::rotate(mMatrix, (float)transform->rotation.z * (float)DEGTORAD, glm::vec3(0.0f, 0.0f, 1.0f));
			mMatrix = glm::scale(mMatrix, glm::vec3(transform->scale));
		}
		
		MaterialComponent* material = (this->GetComponent<MaterialComponent>());
		if(material != nullptr) {	

			material->SetupShaderUniforms(mMatrix, vMatrix, pMatrix, this->scene);
			//Draw
			for(int i=0; i<components.size(); i++) {
				components[i]->OnRender();
			}    
		}
	}
	
	//unbind shader program
	// ogl->glUseProgram(0);
	ogl->glEnable(GL_DEPTH_TEST);
	ogl->glCullFace(GL_BACK);
}

void Object3D::RayMarch(Framebuffer* fb) {

}

void Object3D::LateRender() {

}


void Object3D::DepthRenderPass(LightComponent* light) {
	GETGL

	if(!depthTest) ogl->glDisable(GL_DEPTH_TEST);

	//TransformComponent* transform = (TransformComponent*)(this->transform); 
	
	glm::mat4 mMatrix = transform->GetWorldModelMatrix();

	//Render child objects
	for(int i=0; i<childObjects.size(); i++) {
		if (!childObjects[i]->started) childObjects[i]->Start();
		if (!childObjects[i]->enabled) childObjects[i]->Enable();
		childObjects[i]->Render();
	}
	

	if(transform != nullptr) {
		MaterialComponent* material = (this->GetComponent<MaterialComponent>());
		
		ogl->glUseProgram(material->shader.programShaderObject);

		if(light->type == 0 || light->type == 2) { //Directional light
			//Set MVP matrix from the light POV
			glm::mat4 vMatrix = light->viewMat;
			glm::mat4 pMatrix = light->lightProjection;

			glm::mat4 mvpMatrix = pMatrix * vMatrix * mMatrix;
			int modelViewProjectionMatrixLocation = ogl->glGetUniformLocation(material->shader.programShaderObject, "modelViewProjectionMatrix"); 
			ogl->glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, false, glm::value_ptr(mvpMatrix));
		}
		if(light->type == 1) { //Point light, Set the 6 MVP matrices
			int modelMatrixLocation = ogl->glGetUniformLocation(material->shader.programShaderObject, "modelMatrix"); 
			ogl->glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(mMatrix));
		
            for (unsigned int i = 0; i < 6; ++i) {
				std::string uniformName = "shadowMatrices[" + std::to_string(i) + "]";
				int modelViewProjectionMatrixLocation = ogl->glGetUniformLocation(material->shader.programShaderObject, uniformName.c_str()); 
				ogl->glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, false, glm::value_ptr(light->lightSpaceMatrices[i]));
			}
			int farPlaneLocation = ogl->glGetUniformLocation(material->shader.programShaderObject,"farPlane"); 
			ogl->glUniform1f(farPlaneLocation, light->farClip);

			int lightPosLocation = ogl->glGetUniformLocation(material->shader.programShaderObject, "lightPos"); 
			ogl->glUniform3fv(lightPosLocation, 1, glm::value_ptr(glm::vec3(light->object3D->transform->position)));
		}
		
		if(material != nullptr) {
			//Won't use the MVP matrices of the current object but those of the light. 
			//The needed MVP matrices are already binded before
			material->SetupShaderUniforms(glm::mat4(1), glm::mat4(1), glm::mat4(1), this->scene);
			//Draw
			for(int i=0; i<components.size(); i++) {
				components[i]->OnRender();
			}    
		}
	}
	
	//
	//unbind shader
	//
	
	//unbind shader program
	ogl->glUseProgram(0);
	ogl->glEnable(GL_DEPTH_TEST);
}


Object3D* Object3D::Intersects(Geometry::Ray ray, double& _distance) {
	//get closest collision for childs and itself
	double minDistance = 9999999999999999;
	Object3D* closest = nullptr;

	//This code should be in bounding box : bb->intersectsRay();
	BoundingBoxComponent* bb = this->GetComponent<BoundingBoxComponent>();
	if(bb != nullptr) {
		glm::vec3 min;
		glm::vec3 max;
		bb->GetLocalBounds(&min, &max);

		TransformComponent* transform = this->transform;
		glm::mat4 transformMat = transform->GetWorldTransRotMatrix();
		
		glm::vec3 worldScale = transform->GetWorldScale();
		glm::vec3 minScale = min * worldScale;
		glm::vec3 maxScale = max * worldScale;

		double distance;
		bool intersect = Util::RayBoxTest(ray.origin, ray.direction, transformMat, minScale, maxScale, distance);

		if(intersect && distance < minDistance) {
			minDistance = distance;
			closest = this;
		}
	}

	for(int i=0; i<childObjects.size(); i++) {
		double distance;
		childObjects[i]->Intersects(ray, distance);
		if(distance < minDistance) {
			minDistance = distance;
			closest = childObjects[i];
		}
	}
	
	_distance = minDistance;
	return closest;
}

QJsonObject Object3D::ToJSON() {
	QJsonObject json;
	json["name"] = QString::fromStdString(name);
	json["visible"] = (int)visible;
	json["depthTest"] = (int)depthTest;
	json["isStatic"] = (int)isStatic;
	json["type"] = "BaseObject";

	json["transform"] = transform->ToJSON();

	QJsonArray componentsArray;
	for(int i=0; i<components.size(); i++) {
		QJsonObject jsonComponent = components[i]->ToJSON();
		componentsArray.append(jsonComponent);
	}
	json["components"] = componentsArray;

	if(childObjects.size() > 0) {
		
		QJsonArray childObjectsArray;
		for(int i=0; i<childObjects.size(); i++) {
			QJsonObject jsonObject = childObjects[i]->ToJSON();
			childObjectsArray.append(jsonObject);
		}
		json["childObjects"] = childObjectsArray;
	}
	return json;
}

Object3D* Object3D::FromJSON(QJsonObject json, Scene* scene) {
	Object3D* obj = new Object3D("", scene);

	obj->name = json["name"].toString().toStdString();
	obj->visible = (bool) json["visible"].toInt();
	obj->depthTest = (bool) json["depthTest"].toInt();
	obj->isStatic = (bool) json["isStatic"].toInt();
	QJsonArray componentArray = json["components"].toArray();
	for (int componentIndex = 0; componentIndex < componentArray.size(); ++componentIndex) {
		QJsonObject componentJson = componentArray[componentIndex].toObject();
		Component::FromJSON(componentJson, obj);
	}

	// QJsonArray childrenArray = json["childObjects"].toArray();
	// for (int childrenIndex = 0; childrenIndex < childrenArray.size(); ++childrenIndex) {
	// 	QJsonObject childrenJson = childrenArray[childrenIndex].toObject();
	// 	Object3D* child = Object3D::FromJSON(childrenJson);
	// 	obj->AddObject(child);
	// }

	return obj;

}


void Object3D::SetLayerMask(uint16_t _layerMask) {
	this->layerMask = _layerMask;
}

uint16_t Object3D::GetLayerMask() {
	return this->layerMask;
}

bool Object3D::MatchesMask(uint16_t otherMask) {
	bool matches = (otherMask & layerMask) > 0;
	return matches;
}

void Object3D::SetIsVertex(bool isVertex) {
	this->isVertex = isVertex;
}

bool Object3D::GetIsVertex() {
	return isVertex;
}

void Object3D::ToggleEditing() {
	this->isEdit = !this->isEdit;
}

std::vector<Object3D*> Object3D::GetEditionObjects() {
	return editingObjects;
}

}
}