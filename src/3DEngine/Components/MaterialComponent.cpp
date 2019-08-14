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

	mainLayout = new QVBoxLayout();
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
		materialComponent->shader = scene->standardShaders.shaders[index];
		UpdateShaderParameters();
		scene->triggerRefresh = true;
	});

	ColorPicker* albedoPicker = new ColorPicker("Albedo", 210, 15, 60, 255);
	connect(albedoPicker, &ColorPicker::ColorPicked, this, [this, materialComponent](QColor color) {
		glm::dvec4 albedoVec = glm::dvec4(color.red(), color.green(), color.blue(), color.alpha()) * 0.00392156;
		materialComponent->albedo = albedoVec;
		scene->triggerRefresh = true;
	});

	mainLayout->addWidget(albedoPicker);

	//Albedo tex
	FilePicker* albedoTexPicker = new FilePicker("Albedo Texture");
	mainLayout->addWidget(albedoTexPicker);
	connect(albedoTexPicker, &FilePicker::FileModified, this, [this, materialComponent](QString string) {
		materialComponent->albedoTexStr = string.toStdString();
		scene->triggerRefresh = true;
	});


	//Albedo Map
	FilePicker* specularPicker = new FilePicker("Specular Texture");
	mainLayout->addWidget(specularPicker);
	connect(specularPicker, &FilePicker::FileModified, this, [this](QString string) {
		std::cout << "laod specular texture" << string.toStdString() << std::endl;
		scene->triggerRefresh = true;
	});

	//Normal Map
	FilePicker* normalTexPicker = new FilePicker("Normal Texture");
	mainLayout->addWidget(normalTexPicker);
	connect(normalTexPicker, &FilePicker::FileModified, this, [this](QString string) {
		std::cout << "laod normal texture" << string.toStdString() << std::endl;
		scene->triggerRefresh = true;
	});

	// shaderParametersLayout = new QVBoxLayout();
	// mainLayout->addLayout(shaderParametersLayout);
}

void MaterialInspector::Refresh() {}

void EmptyLayout(QLayout* layout) {
	if (!layout)
		return;
	
	while (layout->count() > 0) {
		std::cout << "Try get new item0"<<std::endl;
		QLayoutItem* item = layout->takeAt(0);
		std::cout << "Try get new item1"<<std::endl;
		if(item != nullptr) {
			std::cout << "Got item "<<std::endl;
			QWidget* widget = item->widget(); 
			if(widget != nullptr) {
				std::cout << "got widget"<<std::endl;
				delete item->widget();
			}
			
			QLayout* layout = item->layout(); 
			if(layout != nullptr) {
				std::cout << "got layout"<<std::endl;
				EmptyLayout(item->layout());
				delete item->layout();
			}
			delete item;
		}
	}
}

void MaterialInspector::UpdateShaderParameters() {
	
	EmptyLayout(shaderParametersLayout);

	// shaderParametersLayout = new QVBoxLayout();
	// if(materialComponent->shader->GetId() == SHADER_IDS::GOURAUD) {
	// 	mainLayout->addLayout(shaderParametersLayout);
	// 	//For Gouraud
	// 	if (materialComponent->shader->GetId() == SHADER_IDS::GOURAUD) {
	// 		CustomSlider* ambientSlider = new CustomSlider(0, 1, 0.01, "Ambient Factor", materialComponent->ambientFactor);
	// 		shaderParametersLayout->addLayout(ambientSlider);
	// 		connect(ambientSlider, &CustomSlider::Modified, this, [this, &ambientSlider](double val) {
	// 			materialComponent->ambientFactor = val;
	// 			scene->triggerRefresh = true;
	// 		});

	// 		CustomSlider* diffuseSlider = new CustomSlider(0, 1, 0.01, "Diffuse Factor", materialComponent->diffuseFactor);
	// 		shaderParametersLayout->addLayout(diffuseSlider);
	// 		connect(diffuseSlider, &CustomSlider::Modified, this, [this, &diffuseSlider](double val) {
	// 			materialComponent->diffuseFactor = val;
	// 			scene->triggerRefresh = true;
	// 		});

	// 		CustomSlider* specularSlider = new CustomSlider(0, 1, 0.01, "Specular Factor", materialComponent->specularFactor);
	// 		shaderParametersLayout->addLayout(specularSlider);
	// 		connect(specularSlider, &CustomSlider::Modified, this, [this, &specularSlider](double val) {
	// 			materialComponent->specularFactor = val;
	// 			scene->triggerRefresh = true;
	// 		});

	// 		CustomSlider* smoothnessSlider = new CustomSlider(0, 2048, 10, "Smoothness", materialComponent->smoothness);
	// 		shaderParametersLayout->addLayout(smoothnessSlider);
	// 		connect(smoothnessSlider, &CustomSlider::Modified, this, [this, &smoothnessSlider](double val) {
	// 			materialComponent->smoothness = val;
	// 			scene->triggerRefresh = true;
	// 		});
	// 		mainLayout->addLayout(shaderParametersLayout);
	// 	}
	// }
}

MaterialComponent::MaterialComponent(Object3D* object) : Component("Material", object), specularTex(), albedoTex(), normalTex() {
    inited= false;
    influence = 1.0;
    albedo = glm::dvec4(0.75, 0.75, 0.75, 1.0);

	ambientFactor = 0.2;
	diffuseFactor = 0.3;
	specularFactor = 1.0;
	smoothness = 8;
}


void MaterialComponent::OnStart(){}
void MaterialComponent::OnEnable(){}
void MaterialComponent::OnUpdate(){
	//std::cout << "isloaded " << albedoTex.loaded << " tex " << albedoTexStr << std::endl;
	if (!albedoTex.loaded && albedoTexStr != "") {
	//	albedoTex = KikooRenderer::CoreEngine::Texture(albedoTexStr, GL_TEXTURE0);
	}
}
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

	int ambientFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "ambientFactor");
	ogl->glUniform1f(ambientFactorLocation, ambientFactor);

	int diffuseFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "diffuseFactor");
	ogl->glUniform1f(diffuseFactorLocation, diffuseFactor);

	int specularFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularFactor");
	ogl->glUniform1f(specularFactorLocation, specularFactor);

	int smoothnessLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "smoothness");
	ogl->glUniform1f(smoothnessLocation, smoothness);


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
        } else {
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
				//std::cout << glm::to_string(transformComponent->GetModelMatrix()) << std::endl;


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