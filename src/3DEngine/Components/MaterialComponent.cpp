#include "MaterialComponent.hpp"
#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "../StandardShaders.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace KikooRenderer {
namespace CoreEngine {

MaterialComponent::MaterialComponent(Object3D* object) : Component("Material", object) {
    inited= false;
    influence = 1.0;
    albedo = glm::dvec4(1.0, 1.0, 1.0, 1.0);
    this->SetTexture("C:/Users/Jacques/Documents/Boulot/2019/3D Models/Cobblestones/Textures/BrickRound0105_5_S.jpg");

}
void MaterialComponent::OnStart(){}
void MaterialComponent::OnEnable(){}
void MaterialComponent::OnUpdate(){}
void MaterialComponent::OnRender(){} 
void MaterialComponent::OnDestroy(){} 

void MaterialComponent::SetTexture(std::string path){
    GETGL
    ogl->glActiveTexture(GL_TEXTURE0);
    ogl->glGenTextures(1, &texture);  
    ogl->glBindTexture(GL_TEXTURE_2D, texture); 

    ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    
    ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    ogl->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0); 
    if (data)
    {
        ogl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        ogl->glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    } 
    stbi_image_free(data);
    ogl->glBindTexture(GL_TEXTURE_2D, 0); 
} 

void MaterialComponent::SetShader(Shader* shader) {
    this->shader = shader;
    inited = true;
}

void MaterialComponent::SetupShaderUniforms(glm::dmat4 modelMatrix, glm::dmat4 viewMatrix, glm::dmat4 projectionMatrix, Scene* scene) {
    GETGL
    
	glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
    ogl->glUseProgram(shader->programShaderObject);
	
    int influenceLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "materialInfluence"); 
	ogl->glUniform1f(influenceLocation, influence);

	int modelViewProjectionMatrixLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "modelViewProjectionMatrix"); 
	ogl->glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, false, glm::value_ptr(mvpMatrix));

    glm::vec3 camPos = scene->camera.transform.position;
    int cameraPosLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "cameraPos"); 
	ogl->glUniform3fv(cameraPosLocation,1, glm::value_ptr(camPos));
    
	int albedoLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "albedo"); 
	ogl->glUniform4fv(albedoLocation, 1, glm::value_ptr(albedo));

    //Set albedo texture
    ogl->glBindTexture(GL_TEXTURE_2D, texture);
    int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "ourTexture"); 
	ogl->glUniform1i(texLocation, 0);
    ogl->glBindTexture(GL_TEXTURE_2D, 0);

    if(this->shader->GetId() != SHADER_IDS::UNLIT) {
        int modelMatrixLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "modelMatrix"); 
        ogl->glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(modelMatrix)));

        int numLights = 0;
        for(int i=0; i<scene->lightObjects.size(); i++) {
            LightComponent* lightComponent = (LightComponent*) scene->lightObjects[i]->GetComponent("Light"); 
            TransformComponent* transformComponent = (TransformComponent*) scene->lightObjects[i]->GetComponent("Transform"); 
            
            if(lightComponent != nullptr) {
                std::string varName = "lights[" + std::to_string(i) + "].type";
                GLuint loc = ogl->glGetUniformLocation(this->shader->programShaderObject, varName.c_str());
                ogl->glUniform1i(loc, lightComponent->type);

                varName = "lights[" + std::to_string(i) + "].position";
                loc = ogl->glGetUniformLocation(this->shader->programShaderObject, varName.c_str());
                ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(transformComponent->position)));

                varName = "lights[" + std::to_string(i) + "].direction";
                loc = ogl->glGetUniformLocation(this->shader->programShaderObject, varName.c_str());
                ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(glm::column(transformComponent->GetModelMatrix(), 2))));


                varName = "lights[" + std::to_string(i) + "].attenuation";
                loc = ogl->glGetUniformLocation(this->shader->programShaderObject, varName.c_str());
                ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(lightComponent->attenuation)));

                varName = "lights[" + std::to_string(i) + "].color";
                loc = ogl->glGetUniformLocation(this->shader->programShaderObject, varName.c_str());
                ogl->glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(lightComponent->color)));

                numLights++;
            }
        }
        int numLightsLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "numLights"); 
        ogl->glUniform1i(numLightsLocation, numLights);


        if(this->shader->GetId() == SHADER_IDS::PBR) {         
            GLuint loc = ogl->glGetUniformLocation(this->shader->programShaderObject, "roughness");
            ogl->glUniform1f(loc, 0.5);

            loc = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularFrac");
            ogl->glUniform1f(loc, 0.5);
        }
    }
    
}
}
}