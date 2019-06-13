#include "Object3D.hpp"
#include "Components/MaterialComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {

namespace CoreEngine {

Object3D::Object3D(std::string _name, Scene* _scene) {
    this->name = _name;
    this->scene = _scene;

	TransformComponent* transform = new TransformComponent(this);
	this->AddComponent(transform);
}

void Object3D::AddComponent(Component* component) {

	for(int i=0; i<components.size(); i++) { 
		if(components[i]->name == component->name) {
			components[i] = component;
			return;
		}
	}	
    this->components.push_back(component);
}

void Object3D::AddObject(Object3D* object) {
    this->childObjects.push_back(object);
}

void Object3D::Start() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Start();
	}
}

void Object3D::Enable() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Enable();
	}
    for(int i=0; i<components.size(); i++) {
        components[i]->OnEnable();
    }
}

void Object3D::Update() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Update();
	}

    for(int i=0; i<components.size(); i++) {
        components[i]->OnUpdate();
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

Component* Object3D::GetComponent(std::string name) {
	for(int i=0; i<components.size(); i++) { 
		if(components[i]->name == name) {
			return components[i];
		}
	}
	return nullptr;
}

void Object3D::Render(glm::mat4* modelMatrix) {
	GETGL
	TransformComponent* transform = (TransformComponent*)(this->GetComponent("Transform")); 
	
	glm::mat4 currentModelMatrix;
	if(modelMatrix != nullptr) currentModelMatrix = (*modelMatrix); //If child of object, modelMatrix is the local coordinate matrix of the parent
	else currentModelMatrix = transform->GetModelMatrix(); //If child of scene, get world position

	//Render child objects
	for(int i=0; i<childObjects.size(); i++) {
		TransformComponent* childTransform = (TransformComponent*)childObjects[i]->GetComponent("Transform");
		glm::mat4 newModelMat = currentModelMatrix *  (glm::mat4)childTransform->GetModelMatrix(); 
		childObjects[i]->Render(&newModelMat);
	}
	

	if(transform != nullptr) {
		glm::mat4 vMatrix = scene->camera.GetViewMatrix();
		glm::mat4 pMatrix = scene->camera.GetProjectionMatrix();
		glm::mat4 mvpMatrix = pMatrix * vMatrix * currentModelMatrix;

		
		//bind shader
		MaterialComponent* material = (MaterialComponent*)(this->GetComponent("Material"));
		if(material != nullptr) {
			material->SetupShaderUniforms(currentModelMatrix, vMatrix, pMatrix, this->scene);
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
}
}
}