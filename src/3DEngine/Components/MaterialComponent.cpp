#include "MaterialComponent.hpp"
#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/StandardShaders.hpp"
#include "3DEngine/Shaders/ShaderParams.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {
namespace CoreEngine {


MaterialInspector::MaterialInspector(MaterialComponent* materialComponent) : ComponentInspector("Material", materialComponent) {
	this->materialComponent = materialComponent;
	Object3D* object = materialComponent->object3D;
	scene = object->scene;

	mainLayout = new QVBoxLayout();
	setLayout(mainLayout);

	//Shader
	QHBoxLayout* shaderLayout = new QHBoxLayout();
	QComboBox* shaderList = new QComboBox();
	QLabel* shaderLabel = new QLabel("Shader");
	for (int i = 0; i < scene->standardShaders.shaders.size(); i++) {
		shaderList->addItem(QString::fromStdString(scene->standardShaders.shaders[i]->name));
	}
	int shaderInx = materialComponent->shader.GetId();
	shaderList->setCurrentIndex(shaderInx);

	shaderLayout->addWidget(shaderLabel);
	shaderLayout->addWidget(shaderList);

	mainLayout->addLayout(shaderLayout);
	connect(shaderList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this, materialComponent](int index) {
		scene->glWindow->makeCurrent();
		Shader shader = (*scene->standardShaders.shaders[index]);
		materialComponent->SetShader(shader);
		UpdateShaderParameters();
		scene->triggerRefresh = true;
		materialComponent->firstIter = true;
		scene->glWindow->doneCurrent();
	});

	ColorPicker* albedoPicker = new ColorPicker("Albedo", 210, 15, 60, 255);
	mainLayout->addWidget(albedoPicker);
	connect(albedoPicker, &ColorPicker::ColorPicked, this, [this, materialComponent](QColor color) {
		glm::vec4 albedoVec = glm::vec4(color.red(), color.green(), color.blue(), color.alpha()) * 0.00392156;
		materialComponent->albedo = albedoVec;
		

		scene->glWindow->makeCurrent();
		GETGL
		ogl->glUseProgram(materialComponent->shader.programShaderObject);
		int albedoLocation = ogl->glGetUniformLocation(materialComponent->shader.programShaderObject, "albedo"); 
		ogl->glUniform4fv(albedoLocation, 1, glm::value_ptr(materialComponent->albedo));		
		scene->triggerRefresh = true;
		scene->glWindow->doneCurrent();
	});


	//Albedo tex
	TexturePicker* albedoTexPicker = new TexturePicker("Albedo Texture");
	mainLayout->addWidget(albedoTexPicker);
	connect(albedoTexPicker, &TexturePicker::FileModified, this, [this, materialComponent](QString string) {
		scene->glWindow->makeCurrent();
		materialComponent->SetAlbedoTex(KikooRenderer::CoreEngine::Texture(string.toStdString(), GL_TEXTURE0));		
		scene->glWindow->doneCurrent();
		scene->triggerRefresh = true;
	});

	QCheckBox* flipNormalsCheckbox = new QCheckBox("Flip Normals"); flipNormalsCheckbox->setChecked(materialComponent->flipNormals);
	mainLayout->addWidget(flipNormalsCheckbox);
	connect(flipNormalsCheckbox, &QCheckBox::stateChanged, this, [this, materialComponent](int state) {
		materialComponent->flipNormals = state > 0;
		
		scene->glWindow->makeCurrent();
		GETGL
		ogl->glUseProgram(materialComponent->shader.programShaderObject);
		int flipNormalsLocation = ogl->glGetUniformLocation(materialComponent->shader.programShaderObject, "flipNormals"); 
		ogl->glUniform1i(flipNormalsLocation, materialComponent->flipNormals);			
		scene->triggerRefresh = true;
		scene->glWindow->doneCurrent();		
	});

	QCheckBox* receiveShadowCheckbox = new QCheckBox("Receive Shadow"); receiveShadowCheckbox->setChecked(materialComponent->receiveShadow);
	mainLayout->addWidget(receiveShadowCheckbox);
	connect(receiveShadowCheckbox, &QCheckBox::stateChanged, this, [this, materialComponent](int state) {
		materialComponent->receiveShadow = state > 0;
		
		scene->glWindow->makeCurrent();
		GETGL
		ogl->glUseProgram(materialComponent->shader.programShaderObject);
		int receiveShadowLocation = ogl->glGetUniformLocation(materialComponent->shader.programShaderObject, "receiveShadow"); 
		ogl->glUniform1i(receiveShadowLocation, (int)materialComponent->receiveShadow);		
		scene->triggerRefresh = true;
		scene->glWindow->doneCurrent();			
	});

	shaderParametersLayout = new QVBoxLayout();
		
	QLayout* paramsLayout = materialComponent->params->GetLayout(); 
	materialComponent->params->scene = scene;
	shaderParametersLayout->addLayout(paramsLayout);

	mainLayout->addLayout(shaderParametersLayout);
}

void MaterialInspector::Refresh() {}

//Refreshes the layout of shader parameters
void MaterialInspector::UpdateShaderParameters() {
	EmptyLayout(shaderParametersLayout);
	shaderParametersLayout = new QVBoxLayout();
	
	materialComponent->params = StandardShaders::GetParamsById(materialComponent->shader.GetId());

	materialComponent->params->shader = &materialComponent->shader;
	materialComponent->params->scene = materialComponent->object3D->scene;

	QLayout* shaderLayout = materialComponent->params->GetLayout(); 
	shaderParametersLayout->addLayout(shaderLayout);

	mainLayout->addLayout(shaderParametersLayout);
}

MaterialComponent::MaterialComponent(Object3D* object) : Component("Material", object), albedoTex(), cubemap()  {
    inited= false;
    influence = 1.0;
    albedo = glm::vec4(0.75, 0.75, 0.75, 1.0);

}


void MaterialComponent::OnStart(){}
void MaterialComponent::OnEnable(){
	params = StandardShaders::GetParamsById(shader.GetId());
	params->shader = &shader;
	inited = true;
}
void MaterialComponent::OnUpdate(){
	if (shouldLoadAlbedo ) {
	}
}
void MaterialComponent::OnRender(){} 
void MaterialComponent::OnDestroy(){} 
void MaterialComponent::Recompute(){} 

ComponentInspector* MaterialComponent::GetInspector() {
	materialInspector = new MaterialInspector(this);
	return materialInspector;
}


void MaterialComponent::SetShader(Shader shader) {
    this->shader = shader;

	params = StandardShaders::GetParamsById(shader.GetId());
	if(params != nullptr) {
		params->shader = &shader;
		params->scene = object3D->scene;
	}

    inited = true;

	GETGL
	ogl->glUseProgram(this->shader.programShaderObject);
	
	int albedoLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "albedo"); 
	ogl->glUniform4fv(albedoLocation, 1, glm::value_ptr(albedo));

	int flipNormalsLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "flipNormals"); 
	ogl->glUniform1i(flipNormalsLocation, this->flipNormals);		

	int receiveShadowLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "receiveShadow"); 
	ogl->glUniform1i(receiveShadowLocation, (int)this->receiveShadow);	

	int hasAlbedoTexLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasAlbedoTex");
	if(albedoTex.loaded) ogl->glUniform1i(hasAlbedoTexLocation, 1);	
	else ogl->glUniform1i(hasAlbedoTexLocation, 0);	
	

	int influenceLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "materialInfluence");
	ogl->glUniform1f(influenceLocation, influence);		
	
	int hasCubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasCubemap");
	if(cubemap.loaded) ogl->glUniform1i(hasCubemapLocation, 1);
	else  ogl->glUniform1i(hasCubemapLocation, 0);
}

void MaterialComponent::SetCubemap(std::vector<std::string> _cubemapFilenames) {
	GETGL
	this->cubemapfilenames = _cubemapFilenames;
	cubemap = Cubemap(cubemapfilenames);

	ogl->glActiveTexture(GL_TEXTURE3);
	cubemap.Use();
	int cubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "cubemapTexture");
	ogl->glUniform1i(cubemapLocation, 3);

	int hasCubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasCubemap");
	ogl->glUniform1i(hasCubemapLocation, 1);
}

void MaterialComponent::SetAlbedoTex(Texture tex) {
	if(albedoTex.glTex != tex.glTex) {
		GETGL
		ogl->glUseProgram(shader.programShaderObject);
		albedoTex.Use(); 
		albedoTex = tex;
		int texLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "albedoTexture");
		ogl->glUniform1i(texLocation, 0);

		int hasAlbedoTexLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasAlbedoTex");
		ogl->glUniform1i(hasAlbedoTexLocation, 1);
	}
}

void MaterialComponent::SetupShaderUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Scene* scene) {
	if(inited) {
		GETGL
		if(shader.isDepthPass) {
			
		} else {
			if(flipNormals) ogl->glCullFace(GL_FRONT);

			glm::mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;
			ogl->glUseProgram(shader.programShaderObject);

			int modelViewProjectionMatrixLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "modelViewProjectionMatrix"); 
			ogl->glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, false, glm::value_ptr(mvpMatrix));
			
			glm::vec3 camPos = scene->camera->transform->position;
			int cameraPosLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "cameraPos"); 
			ogl->glUniform3fv(cameraPosLocation,1, glm::value_ptr(camPos));

			int modelMatrixLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "modelMatrix"); 
			ogl->glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(modelMatrix)));
			
			if(albedoTex.loaded) albedoTex.Use();

			if (cubemap.loaded) {
				ogl->glCullFace(GL_FRONT);
				ogl->glActiveTexture(GL_TEXTURE3);
				cubemap.Use();
			}
			

			if(this->shader.isLit) {

				params->SetUniforms();

				if(this->shader.GetId() != SHADER_IDS::UNLIT) {

					int numLights = 0;
					for(int i=0; i<scene->lightObjects.size(); i++) {
						LightComponent* lightComponent = scene->lightObjects[i]->GetComponent<LightComponent>(); 
						TransformComponent* transformComponent = scene->lightObjects[i]->transform;
						
						if(lightComponent != nullptr) {
							if(transformComponent->hasChanged || firstIter) {
								std::string varName = "lights[" + std::to_string(i) + "].position";
								GLuint loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(transformComponent->position)));

								varName = "lights[" + std::to_string(i) + "].direction";
								loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(glm::column(transformComponent->GetModelMatrix(), 2))));

								transformComponent->hasChanged = false;
							}

							if(lightComponent->hasChanged || firstIter) {
								std::string varName = "lights[" + std::to_string(i) + "].type";
								GLuint loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniform1i(loc, lightComponent->type);


								varName = "lights[" + std::to_string(i) + "].attenuation";
								loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(lightComponent->attenuation)));

								varName = "lights[" + std::to_string(i) + "].color";
								loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(lightComponent->color) * (float)lightComponent->intensity));
								

								if(lightComponent->type == 0 || lightComponent->type == 2) {			
									varName = "lights[" + std::to_string(i) + "].lightSpaceMatrix";
									loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
									ogl->glUniformMatrix4fv(loc, 1, false,  glm::value_ptr(glm::mat4(lightComponent->lightSpaceMatrix)));

									ogl->glActiveTexture(GL_TEXTURE4);
									ogl->glBindTexture(GL_TEXTURE_2D, lightComponent->depthFBO->depthTexture);
									varName = "lights[" + std::to_string(i) + "].depthMap";
									loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
									ogl->glUniform1i(loc, 4);
								}else if(lightComponent->type == 1) {
									ogl->glActiveTexture(GL_TEXTURE5);
									ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, lightComponent->depthCubeFBO->depthCubemap);								
									varName = "lights[" + std::to_string(i) + "].depthCubeMap";
									loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());								
									ogl->glUniform1i(loc, 5);	

									varName = "lights[" + std::to_string(i) + "].farPlane";
									loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
									ogl->glUniform1f(loc, lightComponent->farClip);								
								}
								lightComponent->hasChanged = false;
							}

							if(lightComponent->type == 0 || lightComponent->type == 2) {			
								ogl->glActiveTexture(GL_TEXTURE4);
								ogl->glBindTexture(GL_TEXTURE_2D, lightComponent->depthFBO->depthTexture);
							}
							if(lightComponent->type == 1) {
								ogl->glActiveTexture(GL_TEXTURE5);
								ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, lightComponent->depthCubeFBO->depthCubemap);								
							}

							numLights++;
						}
					}

					int numLightsLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "numLights"); 
					ogl->glUniform1i(numLightsLocation, numLights);


				// 	// if(this->shader.GetId() == SHADER_IDS::PBR) {         
				// 	// 	GLuint loc = ogl->glGetUniformLocation(this->shader.programShaderObject, "roughness");
				// 	// 	ogl->glUniform1f(loc, 0.5);

				// 	// 	loc = ogl->glGetUniformLocation(this->shader.programShaderObject, "specularFrac");
				// 	// 	ogl->glUniform1f(loc, 0.5);
				// 	// }
				}
			}
		}
	}
	if(firstIter) firstIter = false;
}

QJsonObject MaterialComponent::ToJSON() {
	QJsonObject json;
	json["type"] = "Material";
	json["Shader"] = params->ToJSON();
	json["influence"] = influence;

	QJsonObject albedoObj;
	albedoObj["r"] = albedo.x;
	albedoObj["g"] = albedo.y;
	albedoObj["b"] = albedo.z;
	albedoObj["a"] = albedo.w;

	json["albedo"] = albedoObj;
	json["albedoTexture"] = QString::fromStdString(albedoTexStr);

	return json;
}	

}
}