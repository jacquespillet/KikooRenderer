#include "Object3D.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/BoundingComponent.hpp"
#include "Util.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer {

namespace CoreEngine {

Object3D::Object3D(std::string _name, Scene* _scene) {
    this->name = _name;
    this->scene = _scene;

	TransformComponent* transform = new TransformComponent(this);
	this->AddComponent(transform);
}

void Object3D::AddComponent(Component* component) {
	if(component != nullptr) {
		for(int i=0; i<components.size(); i++) { 
			if(components[i]->name == component->name) {
				components[i] = component;
				return;
			}
		}	
		this->components.push_back(component);
	}
}

std::string Object3D::AddObject(Object3D* object) {
	object->parent = this;

	bool nameIsOk = (childObjects.size() == 0);
	std::string currentName = object->name;
	while(!nameIsOk) {
		for(int i=0; i<childObjects.size(); i++) {
			std::string otherName=childObjects[i]->name;
			if(otherName == currentName) {
				currentName = currentName + " (1)";
			} else {
				nameIsOk = true;
			}
		}
	}
	object->name = currentName;
	childObjects.push_back(object);
	
	return currentName;
}

void Object3D::Start() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Start();
	}
	started = true;
}

void Object3D::Enable() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Enable();
	}
    for(int i=0; i<components.size(); i++) {
        components[i]->OnEnable();
    }
	enabled = true;
}

void Object3D::Update() {
	if(hasToRecompute) {
		Recompute();
		
		for(int i=0; i<components.size(); i++) {
			components[i]->OnUpdate();
		}
	}
	
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Update();
	}
}

void Object3D::Recompute() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Recompute();
	}

    for(int i=0; i<components.size(); i++) {
        components[i]->Recompute();
    }
}

void Object3D::Destroy() {
	for(int i=0; i<childObjects.size(); i++) {
		childObjects[i]->Destroy();
	}
    for(int i=0; i<components.size(); i++) {
        components[i]->OnDestroy();
		delete components[i];
    }
}



Component* Object3D::GetComponent(std::string name) {
	for(int i=0; i<components.size(); i++) { 
		if(components[i]->name == name) {
			return components[i];
		}
	}
	return nullptr;
}

void Object3D::Render(glm::mat4* modelMatrix) {
	GETGL
	if(!depthTest) ogl->glDisable(GL_DEPTH_TEST);

	TransformComponent* transform = (TransformComponent*)(this->GetComponent("Transform")); 
	
	glm::mat4 currentModelMatrix;
	if(modelMatrix != nullptr) currentModelMatrix = (*modelMatrix); //If child of object, modelMatrix is the local coordinate matrix of the parent
	else currentModelMatrix = transform->GetModelMatrix(); //If child of scene, get world position

	//Render child objects
	for(int i=0; i<childObjects.size(); i++) {
		TransformComponent* childTransform = (TransformComponent*)childObjects[i]->GetComponent("Transform");
		glm::mat4 newModelMat = currentModelMatrix *  (glm::mat4)childTransform->GetModelMatrix(); 
		childObjects[i]->Render(&newModelMat);
	}
	

	if(transform != nullptr) {
		glm::mat4 vMatrix = scene->camera.GetViewMatrix();
		glm::mat4 pMatrix = scene->camera.GetProjectionMatrix();
		glm::mat4 mvpMatrix = pMatrix * vMatrix * currentModelMatrix;

		
		//bind shader
		MaterialComponent* material = (MaterialComponent*)(this->GetComponent("Material"));
		if(material != nullptr) {
			material->SetupShaderUniforms(currentModelMatrix, vMatrix, pMatrix, this->scene);
			//Draw
			for(int i=0; i<components.size(); i++) {
				components[i]->OnRender();
			}    
		}
	}
	
	//
	//unbind shader
	//
	
	//unbind shader program
	ogl->glUseProgram(0);
	ogl->glEnable(GL_DEPTH_TEST);
}

Object3D* Object3D::Intersects(Geometry::Ray ray, double& _distance) {
	//get closest collision for childs and itself
	double minDistance = 9999999999999999;
	Object3D* closest = nullptr;


	BoundingBoxComponent* bb = (BoundingBoxComponent*) this->GetComponent("BoundingBox");
	if(bb != nullptr) {
		glm::dvec3 min;
		glm::dvec3 max;
		bb->GetLocalBounds(&min, &max);

		TransformComponent* transform = (TransformComponent*) this->GetComponent("Transform");
		glm::dmat4 transformMat = transform->GetTransRotMatrix();
		
		Object3D* currentObject = this;
		while(currentObject->parent != nullptr) {
			TransformComponent* parentTransform = (TransformComponent*) currentObject->parent->GetComponent("Transform");
			glm::dmat4 parentTransformMat = parentTransform->GetTransRotMatrix();
			transformMat = parentTransformMat * transformMat;
			currentObject = currentObject->parent;
		}
		// glm::dvec3 minScale = min * transform->GetScale();
		// glm::dvec3 maxScale = max * transform->GetScale();
		glm::dvec3 minScale = min * transform->scale;
		glm::dvec3 maxScale = max * transform->scale;

		double distance;
		bool intersect = Util::RayBoxTest(ray.origin, ray.direction, transformMat, minScale, maxScale, distance);

		if(intersect && distance < minDistance) {
			minDistance = distance;
			closest = this;
		}
	}

	for(int i=0; i<childObjects.size(); i++) {
		double distance;
		childObjects[i]->Intersects(ray, distance);
		if(distance < minDistance) {
			minDistance = distance;
			closest = childObjects[i];
		}
	}
	
	_distance = minDistance;
	return closest;
}

}
}