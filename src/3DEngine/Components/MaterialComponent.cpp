#include "MaterialComponent.hpp"
#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "3DEngine/Object3D.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {
namespace CoreEngine {

MaterialInspector::MaterialInspector(MaterialComponent* materialComponent) : QGroupBox("Material") {
	this->materialComponent = materialComponent;
	Object3D* object = materialComponent->object3D;
	scene = object->scene;

	QVBoxLayout* mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//Shader
	QHBoxLayout* shaderLayout = new QHBoxLayout();
	QComboBox* shaderList = new QComboBox();
	QLabel* shaderLabel = new QLabel("Shader");
	for (int i = 0; i < scene->standardShaders.numShaders; i++) {
		shaderList->addItem(QString::fromStdString(scene->standardShaders.ids[i]));

	}
	int shaderInx = materialComponent->shader->GetId();
	shaderList->setCurrentIndex(shaderInx);
	shaderLayout->addWidget(shaderLabel);
	shaderLayout->addWidget(shaderList);
	mainLayout->addLayout(shaderLayout);

	connect(shaderList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this, materialComponent](int index) {
		std::cout << "Changge shader" << index << std::endl;
		materialComponent->shader = &scene->standardShaders.gouraudShader;
	});

	ColorPicker* albedoPicker = new ColorPicker("Albedo", 210, 15, 60, 255);
	connect(albedoPicker, &ColorPicker::ColorPicked, this, [this, materialComponent](QColor color) {
		glm::dvec4 albedoVec = glm::dvec4(color.red(), color.green(), color.blue(), color.alpha()) * 0.00392156;
		materialComponent->albedo = albedoVec;
	});

	mainLayout->addWidget(albedoPicker);

	//Albedo tex
	FilePicker* albedoTexPicker = new FilePicker("Albedo Texture");
	mainLayout->addWidget(albedoTexPicker);
	connect(albedoTexPicker, &FilePicker::FileModified, this, [this](QString string) {
		std::cout << "laod qlbedo texture" << string.toStdString() << std::endl;
	});


	//Albedo Map
	FilePicker* specularPicker = new FilePicker("Specular Texture");
	mainLayout->addWidget(specularPicker);
	connect(specularPicker, &FilePicker::FileModified, this, [this](QString string) {
		std::cout << "laod specular texture" << string.toStdString() << std::endl;
	});

	//Normal Map
	FilePicker* normalTexPicker = new FilePicker("Normal Texture");
	mainLayout->addWidget(normalTexPicker);
	connect(normalTexPicker, &FilePicker::FileModified, this, [this](QString string) {
		std::cout << "laod normal texture" << string.toStdString() << std::endl;
	});
}

void MaterialInspector::Refresh() {}

MaterialComponent::MaterialComponent(Object3D* object) : Component("Material", object), specularTex(), albedoTex(), normalTex() {
    inited= false;
    influence = 1.0;
    albedo = glm::dvec4(1.0, 1.0, 1.0, 1.0);
}


void MaterialComponent::OnStart(){}
void MaterialComponent::OnEnable(){}
void MaterialComponent::OnUpdate(){}
void MaterialComponent::OnRender(){} 
void MaterialComponent::OnDestroy(){} 
void MaterialComponent::Recompute(){} 

MaterialInspector* MaterialComponent::GetInspector() {
	materialInspector = new MaterialInspector(this);
	return materialInspector;
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

    glm::vec3 camPos = scene->camera->transform->position;
    int cameraPosLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "cameraPos"); 
	ogl->glUniform3fv(cameraPosLocation,1, glm::value_ptr(camPos));
    
	int albedoLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "albedo"); 
	ogl->glUniform4fv(albedoLocation, 1, glm::value_ptr(albedo));
 

	if (albedoTex.loaded) {
		albedoTex.Use();
		int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "albedoTexture");
		ogl->glUniform1i(texLocation, 0);

		int hasAlbedoTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasAlbedoTex");
		ogl->glUniform1i(hasAlbedoTexLocation, 1);
	}
	else {
		int hasAlbedoTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasAlbedoTex");
		ogl->glUniform1i(hasAlbedoTexLocation, 0);
	}

    if(this->shader->GetId() != SHADER_IDS::UNLIT) {
        int modelMatrixLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "modelMatrix"); 
        ogl->glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(modelMatrix)));

        if(specularTex.loaded) {
            specularTex.Use();
            int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularTexture"); 
            ogl->glUniform1i(texLocation, 1); 

            int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
            ogl->glUniform1i(hasSpecularTexLocation, 1);
        }   else {
            int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
            ogl->glUniform1i(hasSpecularTexLocation, 0);
        }

        if(normalTex.loaded) {
            normalTex.Use();
            int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalTexture"); 
            ogl->glUniform1i(texLocation, 2); 
            
            int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
            ogl->glUniform1i(hasNormalTexLocation, 1);
        }  else {
            int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
            ogl->glUniform1i(hasNormalTexLocation, 0);
        } 

        int numLights = 0;
        for(int i=0; i<scene->lightObjects.size(); i++) {
            LightComponent* lightComponent = (LightComponent*) scene->lightObjects[i]->GetComponent("Light"); 
            TransformComponent* transformComponent = scene->lightObjects[i]->transform; 
            
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