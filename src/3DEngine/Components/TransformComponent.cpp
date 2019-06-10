#include "TransformComponent.hpp"

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

glm::dmat4 TransformComponent::GetModelMatrix() {
	glm::dmat4 translateM = glm::translate(glm::dmat4(1.0f), this->position);
	
	glm::dmat4 scaleM = glm::scale(glm::dmat4(1.0f), scale);

	glm::dmat4 rotxPM = glm::rotate(glm::dmat4(1.0f), rotation.x * DEGTORAD, glm::dvec3(1.0f, 0.0f, 0.0f));//rot x axis
	glm::dmat4 rotyPM = glm::rotate(glm::dmat4(1.0f), rotation.y * DEGTORAD, glm::dvec3(0.0f, 1.0f, 0.0f));//rot y axis
	glm::dmat4 rotzPM = glm::rotate(glm::dmat4(1.0f), rotation.z * DEGTORAD, glm::dvec3(0.0f, 0.0f, 1.0f));//rot z axis

	glm::dmat4 rotM = rotyPM * rotxPM * rotzPM; 	
		
	return translateM * rotM * scaleM;
}

}
}