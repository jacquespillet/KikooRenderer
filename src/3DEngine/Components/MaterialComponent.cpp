#include "MaterialComponent.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

MaterialComponent::MaterialComponent(Object3D* object) : Component("Material", object) {
    inited= false;
}
void MaterialComponent::OnStart(){}
void MaterialComponent::OnEnable(){}
void MaterialComponent::OnUpdate(){}
void MaterialComponent::OnRender(){} 

void MaterialComponent::SetShader(Shader* shader) {
    this->shader = shader;
    inited = true;
}

void MaterialComponent::SetupShaderUniforms(glm::dmat4 modelMatrix, glm::dmat4 viewMatrix, glm::dmat4 projectionMatrix, Scene* scene) {
    GETGL
    
	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
    ogl->glUseProgram(shader->programShaderObject);

	int modelViewProjectionMatrixLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "modelViewProjectionMatrix"); 
	ogl->glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, false, glm::value_ptr(mvpMatrix));
    

    //Shader->GetUniformVariables()->setVariables();
    
	int albedoLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "albedo"); 
	ogl->glUniform4fv(albedoLocation, 1, glm::value_ptr(albedo));
}
}
}