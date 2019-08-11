#include "TransformComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"


namespace KikooRenderer {
namespace CoreEngine {

TransformInspector::TransformInspector(TransformComponent* transformComponent) : QGroupBox("Transform") {
	setLocale(QLocale("C")); //For . as , in double spin boxes

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

	connect(xPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionX(double)));
	connect(yPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionY(double)));
	connect(zPositionSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetPositionZ(double)));

	connect(xRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetRotationX(double)));
	connect(yRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetRotationY(double)));
	connect(zRotationSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetRotationZ(double)));

	connect(xScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleX(double)));
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleY(double)));
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetScaleZ(double)));

	setLayout(mainLayout);
}

void TransformInspector::SetScaleX(double x) { transformComponent->scale.x = x; scene->triggerRefresh = true; }
void TransformInspector::SetScaleY(double y) { transformComponent->scale.y = y; scene->triggerRefresh = true;}
void TransformInspector::SetScaleZ(double z) { transformComponent->scale.z = z; scene->triggerRefresh = true;}

void TransformInspector::SetRotationX(double x) { transformComponent->rotation.x = x; scene->triggerRefresh = true;}
void TransformInspector::SetRotationY(double y) { transformComponent->rotation.y = y; scene->triggerRefresh = true;}
void TransformInspector::SetRotationZ(double z) { transformComponent->rotation.z = z; scene->triggerRefresh = true;}

void TransformInspector::SetPositionX(double x) { transformComponent->position.x = x; scene->triggerRefresh = true;}
void TransformInspector::SetPositionY(double y) { transformComponent->position.y = y; scene->triggerRefresh = true;}
void TransformInspector::SetPositionZ(double z) { transformComponent->position.z = z; scene->triggerRefresh = true;}

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
    position = glm::dvec3(0, 0, 0);
    rotation = glm::dvec3(0, 0, 0);
    scale = glm::dvec3(1, 1, 1);   
}
void TransformComponent::OnStart(){}
void TransformComponent::OnEnable(){}
void TransformComponent::OnUpdate(){}
void TransformComponent::OnRender(){} 
void TransformComponent::OnDestroy(){} 
TransformInspector* TransformComponent::GetInspector() {
	transformInspector = new TransformInspector(this);
	return transformInspector;
}

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

glm::dvec3 TransformComponent::GetWorldScale() {
	glm::dvec3 worldScale = scale;
	Object3D* currentObject = object3D;
	while (currentObject->parent != nullptr) {
		glm::dvec3 parentScale = currentObject->parent->transform->scale;
		worldScale *= parentScale ;
		currentObject = currentObject->parent;
	}
	return worldScale;
}

glm::dmat4 TransformComponent::GetWorldTransRotMatrix() {
	glm::dmat4 transformMat = GetTransRotMatrix();
	Object3D* currentObject = object3D;
	while (currentObject->parent != nullptr) {
		glm::dmat4 parentTransformMat = currentObject->parent->transform->GetTransRotMatrix();
		transformMat = parentTransformMat * transformMat;		
		currentObject = currentObject->parent;
	}

	glm::dvec3 wPos = GetWorldPosition();

	transformMat[3][0] = wPos.x;
	transformMat[3][1] = wPos.y;
	transformMat[3][2] = wPos.z;
	return transformMat;
}


void TransformComponent::SetWorldX(double x) {
	if (object3D->parent == nullptr) {
		this->position.x = x;
	}
	else {
		glm::dvec3 parentPos = object3D->parent->transform->GetWorldPosition();
		double scaleFac = object3D->parent->transform->GetWorldScale().x;
		this->position.x = (x - parentPos.x) / scaleFac;
	}
}

void TransformComponent::SetWorldY(double y) {
	if (object3D->parent == nullptr) {
		this->position.y = y;
	}
	else {
		glm::dvec3 parentPos = object3D->parent->transform->GetWorldPosition();
		double scaleFac = object3D->parent->transform->GetWorldScale().y;
		this->position.y = (y - parentPos.y) / scaleFac;
	}
}

void TransformComponent::SetWorldZ(double z) {
	if (object3D->parent == nullptr) {
		this->position.z = z;
	}
	else {
		glm::dvec3 parentPos = object3D->parent->transform->GetWorldPosition();
		double scaleFac = object3D->parent->transform->GetWorldScale().z;
		this->position.z = (z - parentPos.z) / scaleFac;
	}
}
}
}