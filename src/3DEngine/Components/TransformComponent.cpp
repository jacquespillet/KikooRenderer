#include "TransformComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"


namespace KikooRenderer {
namespace CoreEngine {
TransformComponent::TransformComponent(Object3D* object) : Component("Transform", object) {
    position = glm::dvec3(0, 0, 0);
    rotation = glm::dvec3(0, 0, 0);
    scale = glm::dvec3(1, 1, 1);   
}
void TransformComponent::OnStart(){}
void TransformComponent::OnEnable(){}
void TransformComponent::OnUpdate(){}
void TransformComponent::OnRender(){} 
void TransformComponent::OnDestroy(){} 
void TransformComponent::Recompute() {}

glm::dmat4 TransformComponent::GetModelMatrix() {
	glm::dmat4 translateM = glm::translate(glm::dmat4(1.0f), this->position);

	double distanceFactor = 1;
	// if(isScreenSize) {
	// 	distanceFactor = 0.1 * glm::distance( object3D->scene->camera.GetPosition(), this->position);		
	// }
	
	glm::dmat4 scaleM = glm::scale(glm::dmat4(1.0f), scale * distanceFactor);

	glm::dmat4 rotxPM = glm::rotate(glm::dmat4(1.0f), rotation.x * DEGTORAD, glm::dvec3(1.0f, 0.0f, 0.0f));//rot x axis
	glm::dmat4 rotyPM = glm::rotate(glm::dmat4(1.0f), rotation.y * DEGTORAD, glm::dvec3(0.0f, 1.0f, 0.0f));//rot y axis
	glm::dmat4 rotzPM = glm::rotate(glm::dmat4(1.0f), rotation.z * DEGTORAD, glm::dvec3(0.0f, 0.0f, 1.0f));//rot z axis

	glm::dmat4 rotM = rotyPM * rotxPM * rotzPM; 	
		
	return translateM * rotM * scaleM;
}
glm::dmat4 TransformComponent::GetTransRotMatrix() {
	glm::dmat4 translateM = glm::translate(glm::dmat4(1.0f), this->position);
	
	glm::dmat4 rotxPM = glm::rotate(glm::dmat4(1.0f), rotation.x * DEGTORAD, glm::dvec3(1.0f, 0.0f, 0.0f));//rot x axis
	glm::dmat4 rotyPM = glm::rotate(glm::dmat4(1.0f), rotation.y * DEGTORAD, glm::dvec3(0.0f, 1.0f, 0.0f));//rot y axis
	glm::dmat4 rotzPM = glm::rotate(glm::dmat4(1.0f), rotation.z * DEGTORAD, glm::dvec3(0.0f, 0.0f, 1.0f));//rot z axis

	glm::dmat4 rotM = rotyPM * rotxPM * rotzPM; 	
		
	return translateM * rotM;
}

glm::dvec3 TransformComponent::GetScale() {
	double distanceFactor = 1;

	//Get world position
	glm::dmat4 transformMat = GetWorldModelMatrix();
	glm::dvec4 worldPosition = transformMat * glm::dvec4(0, 0, 0, 1);

	if(isScreenSize) {
		distanceFactor = 0.1 * glm::distance( object3D->scene->camera->GetPosition(), glm::dvec3(worldPosition));		
	}
	
	return distanceFactor * scale;

}

glm::mat4 TransformComponent::GetWorldModelMatrix() {
	//Get world position
	glm::dmat4 transformMat = GetModelMatrix();
	Object3D* currentObject = object3D;
	while(currentObject->parent != nullptr) {
		TransformComponent* parentTransform = currentObject->parent->transform;
		glm::dmat4 parentTransformMat = parentTransform->GetModelMatrix();
		transformMat = parentTransformMat * transformMat;
		currentObject = currentObject->parent;
	}
	return transformMat;
}

glm::dvec3 TransformComponent::GetWorldPosition() {
	glm::dvec3 pos = glm::column(GetWorldModelMatrix(), 3);
	return pos;
}

void TransformComponent::SetWorldX(double x) {
	if (object3D->parent == nullptr) {
		this->position.x = x;
	}
	else {
		glm::dvec3 parentPos = object3D->parent->transform->GetWorldPosition();
		this->position.x = x - parentPos.x;
	}
}

void TransformComponent::SetWorldY(double y) {
	if (object3D->parent == nullptr) {
		this->position.y = y;
	}
	else {
		glm::dvec3 parentPos = object3D->parent->transform->GetWorldPosition();
		this->position.y = y - parentPos.y;
	}
}

void TransformComponent::SetWorldZ(double z) {
	if (object3D->parent == nullptr) {
		this->position.z = z;
	}
	else {
		glm::dvec3 parentPos = object3D->parent->transform->GetWorldPosition();
		this->position.z = z - parentPos.z;
	}
}
}
}