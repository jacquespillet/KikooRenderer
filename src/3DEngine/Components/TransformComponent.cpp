#include "TransformComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"


namespace KikooRenderer {
namespace CoreEngine {

TransformInspector::TransformInspector(TransformComponent* transformComponent) : ComponentInspector("Transform", transformComponent)  {
	setLocale(QLocale("C")); //For . as , in float spin boxes

	this->transformComponent = transformComponent;
	Object3D* object = transformComponent->object3D;
	scene = object->scene;

	QVBoxLayout* mainLayout = new QVBoxLayout();

	//Position
	QLabel* positionLabel = new QLabel("Position");
	QHBoxLayout* positionLayout = new QHBoxLayout();

	QLabel* xPositionLabel = new QLabel("x"); xPositionSpinBox = new QDoubleSpinBox(); 
	xPositionSpinBox->setDecimals(4); xPositionSpinBox->setRange(-500, 500);  xPositionSpinBox->setValue(transformComponent->position.x);  positionLayout->addWidget(xPositionLabel); positionLayout->addWidget(xPositionSpinBox);
	QLabel* yPositionLabel = new QLabel("y"); yPositionSpinBox = new QDoubleSpinBox(); 
	yPositionSpinBox->setDecimals(4); yPositionSpinBox->setRange(-500, 500); yPositionSpinBox->setValue(transformComponent->position.y); positionLayout->addWidget(yPositionLabel);  positionLayout->addWidget(yPositionSpinBox);
	QLabel* zPositionLabel = new QLabel("z"); zPositionSpinBox = new QDoubleSpinBox(); 
	zPositionSpinBox->setDecimals(4); zPositionSpinBox->setRange(-500, 500); zPositionSpinBox->setValue(transformComponent->position.z); positionLayout->addWidget(zPositionLabel);  positionLayout->addWidget(zPositionSpinBox);

	mainLayout->addWidget(positionLabel);
	mainLayout->addLayout(positionLayout);

	//Rotation
	QLabel* rotationLabel = new QLabel("Rotation");
	QHBoxLayout* rotationLayout = new QHBoxLayout();
	QLabel* xRotationLabel = new QLabel("x"); xRotationSpinBox = new QDoubleSpinBox(); 
	xRotationSpinBox->setDecimals(4); xRotationSpinBox->setRange(-500, 500); xRotationSpinBox->setValue(transformComponent->rotation.x); rotationLayout->addWidget(xRotationLabel); rotationLayout->addWidget(xRotationSpinBox);
	QLabel* yRotationLabel = new QLabel("y"); yRotationSpinBox = new QDoubleSpinBox(); 
	yRotationSpinBox->setDecimals(4); yRotationSpinBox->setRange(-500, 500); yRotationSpinBox->setValue(transformComponent->rotation.y); rotationLayout->addWidget(yRotationLabel);  rotationLayout->addWidget(yRotationSpinBox);
	QLabel* zRotationLabel = new QLabel("z"); zRotationSpinBox = new QDoubleSpinBox(); 
	zRotationSpinBox->setDecimals(4); zRotationSpinBox->setRange(-500, 500); zRotationSpinBox->setValue(transformComponent->rotation.z); rotationLayout->addWidget(zRotationLabel);  rotationLayout->addWidget(zRotationSpinBox);

	mainLayout->addWidget(rotationLabel);
	mainLayout->addLayout(rotationLayout);


	//Scale
	QLabel* scaleLabel = new QLabel("Scale");
	QHBoxLayout* scaleLayout = new QHBoxLayout();
	QLabel* xScaleLabel = new QLabel("x"); xScaleSpinBox = new QDoubleSpinBox(); 
	xScaleSpinBox->setDecimals(4); xScaleSpinBox->setRange(-500, 500); xScaleSpinBox->setValue(transformComponent->scale.x); scaleLayout->addWidget(xScaleLabel); scaleLayout->addWidget(xScaleSpinBox);
	QLabel* yScaleLabel = new QLabel("y"); yScaleSpinBox = new QDoubleSpinBox(); 
	yScaleSpinBox->setDecimals(4); yScaleSpinBox->setRange(-500, 500); yScaleSpinBox->setValue(transformComponent->scale.y); scaleLayout->addWidget(yScaleLabel);  scaleLayout->addWidget(yScaleSpinBox);
	QLabel* zScaleLabel = new QLabel("z"); zScaleSpinBox = new QDoubleSpinBox(); 
	zScaleSpinBox->setDecimals(4); zScaleSpinBox->setRange(-500, 500); zScaleSpinBox->setValue(transformComponent->scale.z); scaleLayout->addWidget(zScaleLabel);  scaleLayout->addWidget(zScaleSpinBox);

	mainLayout->addWidget(scaleLabel);
	mainLayout->addLayout(scaleLayout);

	connect(xPositionSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->position.x = value; 
		transformComponent->hasChanged = true;
		scene->transformWidget->RecomputePosition();
		scene->triggerRefresh = true;
    });
	
	connect(yPositionSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->position.y = value; 
		transformComponent->hasChanged = true;
		scene->transformWidget->RecomputePosition();
		scene->triggerRefresh = true;
    });
	
	connect(zPositionSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->position.z = value; 
		transformComponent->hasChanged = true;
		scene->transformWidget->RecomputePosition();
		scene->triggerRefresh = true;
    });


	connect(xRotationSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->rotation.x = value; 
		transformComponent->hasChanged = true;
		scene->triggerRefresh = true;
    });
	
	connect(yRotationSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->rotation.y = value; 
		transformComponent->hasChanged = true;
		scene->triggerRefresh = true;
    });
	
	connect(zRotationSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->rotation.z = value; 
		transformComponent->hasChanged = true;
		scene->triggerRefresh = true;
    });


	connect(xScaleSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->scale.x = value; 
		transformComponent->hasChanged = true;
		scene->triggerRefresh = true;
    });
	
	connect(yScaleSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->scale.y = value; 
		transformComponent->hasChanged = true;
		scene->triggerRefresh = true;
    });
	
	connect(zScaleSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, transformComponent](double value) {
		transformComponent->scale.z = value; 
		transformComponent->hasChanged = true;
		scene->triggerRefresh = true;
    });

	setLayout(mainLayout);
}


void TransformInspector::Refresh() {
	xPositionSpinBox->setValue(transformComponent->position.x);
	yPositionSpinBox->setValue(transformComponent->position.y);
	zPositionSpinBox->setValue(transformComponent->position.z);
	xRotationSpinBox->setValue(transformComponent->rotation.x);
	yRotationSpinBox->setValue(transformComponent->rotation.y);
	zRotationSpinBox->setValue(transformComponent->rotation.z);
	xScaleSpinBox->setValue(transformComponent->scale.x);
	yScaleSpinBox->setValue(transformComponent->scale.y);
	zScaleSpinBox->setValue(transformComponent->scale.z);
}


TransformComponent::TransformComponent(Object3D* object) : Component("Transform", object) {
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1); 
	isInAnimation = false;

	std::cout << "HERE "<< std::endl;
}

void TransformComponent::OnStart(){}
void TransformComponent::OnEnable(){}
void TransformComponent::OnUpdate(){
	AnimateToTransform();
}
void TransformComponent::OnRender(){} 
void TransformComponent::OnDestroy(){} 

ComponentInspector* TransformComponent::GetInspector() {
	transformInspector = new TransformInspector(this);
	return transformInspector;
}

void TransformComponent::Recompute() {}

glm::mat4 TransformComponent::GetModelMatrix() {
	glm::mat4 translateM = glm::translate(glm::mat4(1.0f), this->position);

	float distanceFactor = 1;
	// if(isScreenSize) {
	// 	distanceFactor = 0.1 * glm::distance( object3D->scene->camera.GetPosition(), this->position);		
	// }
	
	glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), scale * distanceFactor);

	glm::mat4 rotxPM = glm::rotate(glm::mat4(1.0f), rotation.x * (float)DEGTORAD, glm::vec3(1.0f, 0.0f, 0.0f));//rot x axis
	glm::mat4 rotyPM = glm::rotate(glm::mat4(1.0f), rotation.y * (float)DEGTORAD, glm::vec3(0.0f, 1.0f, 0.0f));//rot y axis
	glm::mat4 rotzPM = glm::rotate(glm::mat4(1.0f), rotation.z * (float)DEGTORAD, glm::vec3(0.0f, 0.0f, 1.0f));//rot z axis

	glm::mat4 rotM = rotyPM * rotxPM * rotzPM; 	
		
	return translateM * rotM * scaleM;
}
glm::mat4 TransformComponent::GetWorldRotationMatrix() {
	glm::mat4 rotxPM = glm::rotate(glm::mat4(1.0f), rotation.x * (float)DEGTORAD, glm::vec3(1.0f, 0.0f, 0.0f));//rot x axis
	glm::mat4 rotyPM = glm::rotate(glm::mat4(1.0f), rotation.y * (float)DEGTORAD, glm::vec3(0.0f, 1.0f, 0.0f));//rot y axis
	glm::mat4 rotzPM = glm::rotate(glm::mat4(1.0f), rotation.z * (float)DEGTORAD, glm::vec3(0.0f, 0.0f, 1.0f));//rot z axis

	glm::mat4 rotM = rotyPM * rotxPM * rotzPM; 	
		
	return rotM;
}
glm::mat4 TransformComponent::GetTransRotMatrix() {
	glm::mat4 translateM = glm::translate(glm::mat4(1.0f), this->position);
	
	glm::mat4 rotxPM = glm::rotate(glm::mat4(1.0f), rotation.x * (float)DEGTORAD, glm::vec3(1.0f, 0.0f, 0.0f));//rot x axis
	glm::mat4 rotyPM = glm::rotate(glm::mat4(1.0f), rotation.y * (float)DEGTORAD, glm::vec3(0.0f, 1.0f, 0.0f));//rot y axis
	glm::mat4 rotzPM = glm::rotate(glm::mat4(1.0f), rotation.z * (float)DEGTORAD, glm::vec3(0.0f, 0.0f, 1.0f));//rot z axis

	glm::mat4 rotM = rotyPM * rotxPM * rotzPM; 	
		
	return translateM * rotM;
}

glm::vec3 TransformComponent::GetScale() {
	float distanceFactor = 1;

	//Get world position
	glm::mat4 transformMat = GetWorldModelMatrix();
	glm::vec4 worldPosition = transformMat * glm::vec4(0, 0, 0, 1);

	if(isScreenSize) {
		distanceFactor = 0.1 * glm::distance( object3D->scene->camera->GetPosition(), glm::vec3(worldPosition));		
	}
	
	return distanceFactor * scale;

}

glm::mat4 TransformComponent::GetWorldModelMatrix() {
	//Get world position
	glm::mat4 transformMat = GetModelMatrix();
	Object3D* currentObject = object3D;
	while(currentObject->parent != nullptr) {
		TransformComponent* parentTransform = currentObject->parent->transform;
		glm::mat4 parentTransformMat = parentTransform->GetModelMatrix();
		transformMat = parentTransformMat * transformMat;
		currentObject = currentObject->parent;
	}
	return transformMat;
}

glm::vec3 TransformComponent::GetWorldPosition() {
	glm::vec3 pos = glm::column(GetWorldModelMatrix(), 3);
	return pos;
}

glm::vec3 TransformComponent::GetWorldScale() {
	glm::vec3 worldScale = scale;
	Object3D* currentObject = object3D;
	while (currentObject->parent != nullptr) {
		glm::vec3 parentScale = currentObject->parent->transform->scale;
		worldScale *= parentScale ;
		currentObject = currentObject->parent;
	}
	return worldScale;
}

glm::mat4 TransformComponent::GetWorldTransRotMatrix() {
	glm::mat4 transformMat = GetTransRotMatrix();
	Object3D* currentObject = object3D;
	while (currentObject->parent != nullptr) {
		glm::mat4 parentTransformMat = currentObject->parent->transform->GetTransRotMatrix();
		transformMat = parentTransformMat * transformMat;		
		currentObject = currentObject->parent;
	}

	glm::vec3 wPos = GetWorldPosition();

	transformMat[3][0] = wPos.x;
	transformMat[3][1] = wPos.y;
	transformMat[3][2] = wPos.z;
	return transformMat;
}


glm::mat4 TransformComponent::GetWorldTransScaleMatrix() {
	glm::vec3 worldPos = position;
	glm::vec3 worldScale = scale;

	Object3D* currentObject = object3D;
	while (currentObject->parent != nullptr) {
		glm::vec3 parentScale = currentObject->parent->transform->scale;
		glm::vec3 parentPos = currentObject->parent->transform->position;
		worldScale *= parentScale ;
		worldPos += parentPos;
		currentObject = currentObject->parent;
	}

	glm::mat4 translateM = glm::translate(glm::mat4(1.0f), worldPos);
	glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), worldScale);
	return translateM * scaleM;
}


void TransformComponent::SetWorldX(float x) {
	if (object3D->parent == nullptr) {
		this->position.x = x;
	}
	else {
		glm::vec3 parentPos = object3D->parent->transform->GetWorldPosition();
		float scaleFac = object3D->parent->transform->GetWorldScale().x;
		this->position.x = (x - parentPos.x) / scaleFac;
	}
	hasChanged = true;
}

void TransformComponent::SetWorldY(float y) {
	if (object3D->parent == nullptr) {
		this->position.y = y;
	}
	else {
		glm::vec3 parentPos = object3D->parent->transform->GetWorldPosition();
		float scaleFac = object3D->parent->transform->GetWorldScale().y;
		this->position.y = (y - parentPos.y) / scaleFac;
	}
	hasChanged = true;
}

void TransformComponent::SetWorldZ(float z) {
	if (object3D->parent == nullptr) {
		this->position.z = z;
	}
	else {
		glm::vec3 parentPos = object3D->parent->transform->GetWorldPosition();
		float scaleFac = object3D->parent->transform->GetWorldScale().z;
		this->position.z = (z - parentPos.z) / scaleFac;
	}
	hasChanged = true;
}
void TransformComponent::StartAnimate(glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) {
	isInAnimation = true;
	transitionFactor = 0;
	initialTransform.position = this->position;
	initialTransform.rotation = this->rotation;
	initialTransform.scale = this->scale;

	targetTransform.position = _position;
	targetTransform.rotation = _rotation;
	targetTransform.scale = _scale;
}

void TransformComponent::AnimateToTransform() {
	if(isInAnimation) {
		transitionFactor += object3D->scene->deltaTime;
		float t = transitionFactor / transitionTime;

		this->position.x = initialTransform.position.x * (1 - t) + targetTransform.position.x * (t);
		this->position.y = initialTransform.position.y * (1 - t) + targetTransform.position.y * (t);
		this->position.z = initialTransform.position.z * (1 - t) + targetTransform.position.z * (t);
		object3D->scene->triggerRefresh = true;
		if(t >= 1) {
			isInAnimation = false;
			transitionFactor = 0;
		} 
	}
}

void TransformComponent::FromJSON(QJsonObject json, Object3D* obj) {

	glm::vec3 position;
	QJsonObject positionJson = json["position"].toObject();
	position.x = positionJson["X"].toDouble();
	position.y = positionJson["Y"].toDouble();
	position.z = positionJson["Z"].toDouble();
	
	glm::vec3 rotation;
	QJsonObject rotationJson = json["rotation"].toObject();
	rotation.x = rotationJson["X"].toDouble();
	rotation.y = rotationJson["Y"].toDouble();
	rotation.z = rotationJson["Z"].toDouble();
	
	glm::vec3 scale;
	QJsonObject scaleJson = json["scale"].toObject();
	scale.x = scaleJson["X"].toDouble();
	scale.y = scaleJson["Y"].toDouble();
	scale.z = scaleJson["Z"].toDouble();

	TransformComponent* transform = new TransformComponent(obj);
	transform->position = position;
	transform->rotation = rotation;
	transform->scale = scale;
	obj->AddComponent(transform);
}

QJsonObject TransformComponent::ToJSON() {
	QJsonObject json;
	json["type"] = "Transform";
	
	QJsonObject positionJson;
	positionJson["X"] = position.x;
	positionJson["Y"] = position.y;
	positionJson["Z"] = position.z;
	json["position"] = positionJson;
	
	QJsonObject scaleJson;
	scaleJson["X"] = scale.x;
	scaleJson["Y"] = scale.y;
	scaleJson["Z"] = scale.z;
	json["scale"] = scaleJson;
	
	QJsonObject rotationJson;
	rotationJson["X"] = rotation.x;
	rotationJson["Y"] = rotation.y;
	rotationJson["Z"] = rotation.z;
	json["rotation"] = rotationJson;

	return json;
}
}
}