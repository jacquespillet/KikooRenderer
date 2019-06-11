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
}

void Object3D::AddComponent(Component* component) {
    this->components.push_back(component);
}
void Object3D::Start() {

}

void Object3D::Enable() {
    for(int i=0; i<components.size(); i++) {
        components[i]->OnEnable();
    }
}

void Object3D::Update() {
    for(int i=0; i<components.size(); i++) {
        components[i]->OnUpdate();
    }
}

void Object3D::Destroy() {
    for(int i=0; i<components.size(); i++) {
        components[i]->OnDestroy();
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

void Object3D::Render() {
	GETGL

	TransformComponent* transform = (TransformComponent*)(this->GetComponent("Transform")); 
	glm::mat4 mMatrix = transform->GetModelMatrix();
	
	glm::mat4 vMatrix = scene->camera.GetViewMatrix();
	glm::mat4 pMatrix = scene->camera.GetProjectionMatrix();
	glm::mat4 mvpMatrix = pMatrix * vMatrix * mMatrix;

	
	//bind shader
	MaterialComponent* material = (MaterialComponent*)(this->GetComponent("Material"));
	if(material == nullptr) {
		material = new MaterialComponent(this);
		material->SetShader(&scene->standardShaders.unlitMeshShader);
		this->AddComponent(material);
	}
	material->SetupShaderUniforms(mMatrix, vMatrix, pMatrix, this->scene);

	//
	//Draw
	//
    for(int i=0; i<components.size(); i++) {
        components[i]->OnRender();
    }    
	
	//
	//unbind shader
	//
	
	//unbind shader program
	ogl->glUseProgram(0);    


}
}
}