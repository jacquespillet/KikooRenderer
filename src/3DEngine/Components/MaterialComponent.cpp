#include "MaterialComponent.hpp"
#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/StandardShaders.hpp"
#include "3DEngine/Shaders/ShaderParams.hpp"
#include "../Shaders/BlinnPhongShader.hpp"

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
	TexturePicker* albedoTexPicker = new TexturePicker("Albedo Texture", materialComponent->albedoTexStr);
	mainLayout->addWidget(albedoTexPicker);
	connect(albedoTexPicker, &TexturePicker::FileModified, this, [this, materialComponent](QString string) {
		materialComponent->albedoTexStr = string.toStdString();
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
	params->scene = object3D->scene;
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
	if(cubemap.GetLoaded()) ogl->glUniform1i(hasCubemapLocation, 1);
	else  ogl->glUniform1i(hasCubemapLocation, 0);
}

void MaterialComponent::SetCubemap(std::vector<std::string> _cubemapFilenames) {
	GETGL
	this->cubemapfilenames = _cubemapFilenames;
	cubemap = Cubemap(cubemapfilenames);

	// ogl->glActiveTexture(GL_TEXTURE3);
	// cubemap.Use();
	// int cubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "cubemapTexture");
	// ogl->glUniform1i(cubemapLocation, 3);

	// int hasCubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasCubemap");
	// ogl->glUniform1i(hasCubemapLocation, 1);
	int hasCubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasCubemap");
	ogl->glUniform1i(hasCubemapLocation, 1);
	
	ogl->glActiveTexture(GL_TEXTURE3);
	cubemap.Use();
	int cubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "cubemapTexture");
	ogl->glUniform1i(cubemapLocation, 3);		
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

			//General uniform that have to be updated each frame
			int modelViewProjectionMatrixLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "modelViewProjectionMatrix"); 
			ogl->glUniformMatrix4fv(modelViewProjectionMatrixLocation, 1, false, glm::value_ptr(mvpMatrix));
			
			glm::vec3 camPos = scene->camera->transform->position;
			int cameraPosLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "cameraPos"); 
			ogl->glUniform3fv(cameraPosLocation,1, glm::value_ptr(camPos));

			int modelMatrixLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "modelMatrix"); 
			ogl->glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(glm::mat4(modelMatrix)));
			if(albedoTex.loaded) albedoTex.Use();

			if (cubemap.GetLoaded()) {

				int hasCubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "hasCubemap");
				ogl->glUniform1i(hasCubemapLocation, 1);

				ogl->glCullFace(GL_FRONT);
				ogl->glActiveTexture(GL_TEXTURE3);
				cubemap.Use();				
				
				int cubemapLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "cubemapTexture");
				ogl->glUniform1i(cubemapLocation, 3);	
			}
			

			params->SetUniforms();
			if(this->shader.isLit) {
				int numLights = 0;
				for(int i=0; i<scene->lightObjects.size(); i++) {
					LightComponent* lightComponent = scene->lightObjects[i]->GetComponent<LightComponent>(); 
					TransformComponent* transformComponent = scene->lightObjects[i]->transform;
					if(lightComponent != nullptr) {
						//Only update the light uniforms if something changed OR if it is the first iteration of the material
						if(transformComponent->hasChanged || lightComponent->hasChanged || firstIter) {
							std::cout << object3D->name << transformComponent->hasChanged << "  "<< lightComponent->hasChanged << " " << firstIter <<  std::endl;
   
							std::string varName = "lights[" + std::to_string(i) + "].position";
							GLuint loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(transformComponent->position)));

							varName = "lights[" + std::to_string(i) + "].direction";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(glm::column(transformComponent->GetModelMatrix(), 2))));

							varName = "lights[" + std::to_string(i) + "].type";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform1i(loc, lightComponent->type);

							
							varName = "lights[" + std::to_string(i) + "].shadowType";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform1i(loc, lightComponent->GetShadowType());
							
							varName = "lights[" + std::to_string(i) + "].numSamples";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform1i(loc, lightComponent->GetNumSamples());

							varName = "lights[" + std::to_string(i) + "].poissonDiskFactor";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform1i(loc, lightComponent->GetPoissonDiskFactor());

							varName = "lights[" + std::to_string(i) + "].bias";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform1f(loc, lightComponent->GetBias());


							varName = "lights[" + std::to_string(i) + "].attenuation";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(lightComponent->attenuation)));

							varName = "lights[" + std::to_string(i) + "].color";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(lightComponent->color) * (float)lightComponent->intensity));
							

							varName = "lights[" + std::to_string(i) + "].farPlane";
							loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
							ogl->glUniform1f(loc, lightComponent->farClip);	

							if(lightComponent->type == 0 || lightComponent->type == 2) {			
								varName = "lights[" + std::to_string(i) + "].lightSpaceMatrix";
								loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniformMatrix4fv(loc, 1, false,  glm::value_ptr(glm::mat4(lightComponent->lightSpaceMatrix)));

								ogl->glActiveTexture(GL_TEXTURE4 + i);
								ogl->glBindTexture(GL_TEXTURE_2D, lightComponent->depthFBO->depthTexture);
								varName = "lights[" + std::to_string(i) + "].depthMap";
								loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());
								ogl->glUniform1i(loc, 4 + i);
							}else if(lightComponent->type == 1) {
								ogl->glActiveTexture(GL_TEXTURE4 + i);
								ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, lightComponent->depthCubeFBO->GetDepthCubemap());								
								varName = "lights[" + std::to_string(i) + "].depthCubeMap";
								loc = ogl->glGetUniformLocation(this->shader.programShaderObject, varName.c_str());								
								ogl->glUniform1i(loc, 4 + i);	

							}
						} else {
							ogl->glActiveTexture(GL_TEXTURE4 + i);
							if(lightComponent->type == 0 || lightComponent->type == 2) {			
								ogl->glBindTexture(GL_TEXTURE_2D, lightComponent->depthFBO->depthTexture);
							}

							if(lightComponent->type == 1) {
								ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, lightComponent->depthCubeFBO->GetDepthCubemap());								
							}
						}
						numLights++;
					}
				}
				
				int numLightsLocation = ogl->glGetUniformLocation(this->shader.programShaderObject, "numLights"); 
				ogl->glUniform1i(numLightsLocation, numLights);
			}
		}
	}
}

QJsonObject MaterialComponent::ToJSON() {
	QJsonObject json;
	json["type"] = "Material";
	json["shader"] = params->ToJSON();
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

void MaterialComponent::FromJSON(QJsonObject json, Object3D* obj) {
	MaterialComponent* material = new MaterialComponent(obj);
    
	QJsonObject albedoObj = json["albedo"].toObject();
	material->albedo.x = albedoObj["r"].toDouble();
	material->albedo.y = albedoObj["g"].toDouble();
	material->albedo.z = albedoObj["b"].toDouble();
	material->albedo.w = albedoObj["a"].toDouble();

	material->albedoTexStr = json["albedoTexture"].toString().toStdString();

	QJsonObject shaderJson = json["shader"].toObject();

	std::string shaderType = shaderJson["type"].toString().toStdString();
	if(shaderType == "Unlit") {
		Shader shader = obj->scene->standardShaders.unlitMeshShader;
		material->SetShader(shader);
	} else if(shaderType == "BlinnPhong") {
		Shader shader = obj->scene->standardShaders.blinnPhongShader;
		material->SetShader(shader);

		BlinnPhongParams* params = (BlinnPhongParams*) material->params;
		params->ambientFactor = shaderJson["ambientFactor"].toDouble();
		params->diffuseFactor = shaderJson["diffuseFactor"].toDouble();
		params->specularFactor = shaderJson["specularFactor"].toDouble();
		params->smoothness = shaderJson["smoothness"].toDouble();
		params->normalMapInfluence = shaderJson["normalMapInfluence"].toDouble();
		params->normalMapStr = shaderJson["normalMapStr"].toDouble();
		params->specularMapStr = shaderJson["specularMapStr"].toDouble();

		QJsonObject specularColorJson = shaderJson["specularColor"].toObject();
		params->specularColor.x = specularColorJson["r"].toDouble();
		params->specularColor.y = specularColorJson["g"].toDouble();
		params->specularColor.z = specularColorJson["b"].toDouble();
		params->specularColor.w = specularColorJson["a"].toDouble();


	} else if(shaderType == "Gouraud") {
		Shader shader = obj->scene->standardShaders.gouraudShader;
		material->SetShader(shader);
	} else if(shaderType == "PBR") {
		Shader shader = obj->scene->standardShaders.PBRShader;
		material->SetShader(shader);
	}


	obj->AddComponent(material);
}

}
}