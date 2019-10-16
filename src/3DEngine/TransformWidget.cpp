#include "TransformWidget.hpp"
#include "BaseObjects.hpp"
#include "Components/TransformComponent.hpp"
#include "3DEngine/Scene.hpp"
#include "Util.hpp"

namespace KikooRenderer {
namespace CoreEngine {

	TransformWidget::TransformWidget(Scene* scene) : Object3D("TransformWidget", scene) {
		transformMode = TransformMode::TRANSLATE;
		{
			translateObject = new Object3D("TranslateWidget", scene);

			Object3D* xcone = GetCone(scene, "coneX", glm::vec3(1, 0, 0), glm::vec3(0, 90, 0), glm::vec3(1), glm::vec4(1.0, 0.0, 0.0, 1.0), true);
			Object3D* xline = GetLine(scene, "lineX", glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec4(1.0, 0.0, 0.0, 1.0), true);
			xcone->depthTest = false;
			xline->depthTest = false;

			translateObject->AddObject(xcone);
			translateObject->AddObject(xline);

			Object3D* ycone = GetCone(scene, "coneY", glm::vec3(0, 1, 0), glm::vec3(-90, 0, 0), glm::vec3(1), glm::vec4(0.0, 1.0, 0.0, 1.0), true);
			Object3D* yline = GetLine(scene, "lineY", glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec4(0.0, 1.0, 0.0, 1.0), true);
			ycone->depthTest = false;
			yline->depthTest = false;

			translateObject->AddObject(ycone);
			translateObject->AddObject(yline);

			Object3D* zcone = GetCone(scene, "coneZ", glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(0.0, 0.0, 1.0, 1.0), true);
			Object3D* zline = GetLine(scene, "lineZ", glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec4(0.0, 0.0, 1.0, 1.0), true);
			zcone->depthTest = false;
			zline->depthTest = false;

			translateObject->AddObject(zcone);
			translateObject->AddObject(zline);

			translateObject->transform->position = glm::vec3(0);
			translateObject->transform->rotation = glm::vec3(0);
			translateObject->transform->scale = glm::vec3(1);
			translateObject->transform->isScreenSize = true;

			AddObject(translateObject);
		}

		{
			scaleObject = new Object3D("ScaleWidget", scene);

			Object3D* xcube = GetCube(scene, "cubeX", glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.5), glm::vec4(1.0, 0.0, 0.0, 1.0));
			Object3D* xline = GetLine(scene, "lineX", glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec4(1.0, 0.0, 0.0, 1.0));
			xcube->depthTest = false;
			xline->depthTest = false;

			scaleObject->AddObject(xcube);
			scaleObject->AddObject(xline);

			Object3D* ycube = GetCube(scene, "cubeY", glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0.5), glm::vec4(0.0, 1.0, 0.0, 1.0));
			Object3D* yline = GetLine(scene, "lineY", glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec4(0.0, 1.0, 0.0, 1.0));
			ycube->depthTest = false;
			yline->depthTest = false;

			scaleObject->AddObject(ycube);
			scaleObject->AddObject(yline);

			Object3D* zcube = GetCube(scene, "cubeZ", glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0.5), glm::vec4(0.0, 0.0, 1.0, 1.0));
			Object3D* zline = GetLine(scene, "lineZ", glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec4(0.0, 0.0, 1.0, 1.0));
			zcube->depthTest = false;
			zline->depthTest = false;

			scaleObject->AddObject(zcube);
			scaleObject->AddObject(zline);

			scaleObject->transform->position = glm::vec3(0);
			scaleObject->transform->rotation = glm::vec3(0);
			scaleObject->transform->scale = glm::vec3(1);
		}

		{
			rotateObject = new Object3D("RotationWidget", scene);

			Object3D* xCircle = GetWireCircle(scene, "CircleX", glm::vec3(0, 0, 0), glm::vec3(90, 0, 0), glm::vec3(1), glm::vec4(1.0, 0.0, 0.0, 1.0));
			xCircle->depthTest = false;
			rotateObject->AddObject(xCircle);

			Object3D* yCircle = GetWireCircle(scene, "CircleY", glm::vec3(0, 0, 0), glm::vec3(0, 90, 0), glm::vec3(1), glm::vec4(0.0, 1.0, 0.0, 1.0));
			yCircle->depthTest = false;
			rotateObject->AddObject(yCircle);

			Object3D* zCircle = GetWireCircle(scene, "CircleZ", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(0.0, 0.0, 1.0, 1.0));
			zCircle->depthTest = false;
			rotateObject->AddObject(zCircle);
		}
		this->transform->scale = 6 * glm::vec3(scene->camera->GetScreenScale(this->transform->position));
		this->visible = false;
	}

	void TransformWidget::AddHandleObject(Object3D* object) {
		if(std::find(currentObjects.begin(), currentObjects.end(), object) == currentObjects.end()) {
			currentObjects.push_back(object);
			TransformComponent* objectTransform = object->transform;
			
			RecomputePosition();
			
			visible = true;		
		}
	}

	void TransformWidget::RemoveHandleObject(Object3D* object) {
        for(int i=0; i<currentObjects.size(); i++) {
            if(currentObjects[i] == object) {
                delete currentObjects[i];
                currentObjects.erase(currentObjects.begin() + i);
                break;
            }
        }
		RecomputePosition();
	}
	
	void TransformWidget::RecomputePosition() {
		glm::vec3 averagePos(0);
		for(int i=0; i<currentObjects.size(); i++) {
			averagePos += currentObjects[i]->transform->position;
		}
		averagePos /= currentObjects.size();
		transform->position = averagePos;		
	}

	void TransformWidget::Disable() {
		currentObjects.resize(0);
		visible = false;
	}

	
	void TransformWidget::Update() {
		if(currentObjects.size() > 0) {
			this->transform->scale = 6 * glm::vec3(scene->camera->GetScreenScale(this->transform->position));
		}
	}	

	void TransformWidget::StartTransform(Object3D* object) {
		visible = true;
		isTransforming = true;
		isFirstFrame = true;
		if      (object->name == "coneX" || object->name == "cubeX" || object->name == "CircleX") axis = TransformAxis::X;
		else if (object->name == "coneY" || object->name == "cubeY" || object->name == "CircleY") axis = TransformAxis::Y;
		else if (object->name == "coneZ" || object->name == "cubeZ" || object->name == "CircleZ") axis = TransformAxis::Z;
	}

	void TransformWidget::OnMouseMoveEvent(QMouseEvent* e) {
		if (isTransforming) {
			if(currentObjects.size() > 0) {
				if(transformMode == TransformMode::TRANSLATE) {
					HandleTranslate(e);
				} else if (transformMode == TransformMode::SCALE) {
					HandleScale(e);
				} else if (transformMode == TransformMode::ROTATE) {
					HandleRotate(e);
				}
				scene->objectDetailsPanel->Refresh();
			}
		}
	}

	void TransformWidget::HandleTranslate(QMouseEvent* e) {
		int newX = e->x();
		int newY = e->y();

		
		Geometry::Ray ray = scene->camera->GetRay(newX, newY);
		double dotX = std::abs(glm::dot(ray.direction, glm::vec3(1, 0, 0)));
		double dotY = std::abs(glm::dot(ray.direction, glm::vec3(0, 1, 0)));
		double dotZ = std::abs(glm::dot(ray.direction, glm::vec3(0, 0, 1)));

		if (axis == TransformAxis::X) {
			double ysign = transform->position.y > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::vec4 plane = (dotY > dotZ) ? glm::vec4(0, ysign * 1, 0, std::abs(transform->position.y)) : glm::vec4(0, 0, zsign * 1, std::abs(transform->position.z));
			double t = -glm::dot(plane, glm::vec4(ray.origin, 1.0)) / glm::dot(plane, glm::vec4(ray.direction, 0.0));

			//Get position of ray on the plane
			glm::vec3 position = ray.origin + t * ray.direction;
			if (isFirstFrame) { //First frame of transformation, get offset btw click and origin of transform widget
				isFirstFrame = false;
				transformOffset = position.x - transform->position.x;
			}
			else {
				//Set transform to x position of intersection
				double newX = position.x - transformOffset;
				
				double difference = newX - transform->position.x; //Difference btw click and previous position
				transform->position.x += difference;

				for(int i=0; i<currentObjects.size(); i++) {
					currentObjects[i]->transform->position.x += difference;
					currentObjects[i]->transform->hasChanged = true;
				}
			}
		}
		else if (axis == TransformAxis::Y) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::vec4 plane = (dotX > dotZ) ? glm::vec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::vec4(0, 0, zsign * 1, std::abs(transform->position.z));

			double t = -glm::dot(plane, glm::vec4(ray.origin, 1.0)) / glm::dot(plane, glm::vec4(ray.direction, 0.0));
			glm::vec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;
				transformOffset = position.y - transform->position.y;
			}
			else {
				double newY = position.y - transformOffset;
				
				double difference = newY - transform->position.y; //Difference btw click and previous position
				transform->position.y += difference;

				for(int i=0; i<currentObjects.size(); i++) {
					currentObjects[i]->transform->position.y += difference;
					currentObjects[i]->transform->hasChanged = true;
				}
			}
		}
		else if (axis == TransformAxis::Z) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double ysign = transform->position.y > 0 ? -1 : 1;
			glm::vec4 plane = (dotX > dotY) ? glm::vec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::vec4(0, ysign * 1, 0, std::abs(transform->position.y));

			double t = -glm::dot(plane, glm::vec4(ray.origin, 1.0)) / glm::dot(plane, glm::vec4(ray.direction, 0.0));
			glm::vec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;

				transformOffset = position.z - transform->position.z;
			}
			else {
				double newZ = position.z - transformOffset;
				
				double difference = newZ - transform->position.z; //Difference btw click and previous position
				transform->position.z += difference;

				for(int i=0; i<currentObjects.size(); i++) {
					currentObjects[i]->transform->position.z += difference;
					currentObjects[i]->transform->hasChanged = true;
				}
			}
		}
	}

	void TransformWidget::HandleRotate(QMouseEvent* e) {
		int newX = e->x();
		int newY = e->y();
		Geometry::Ray ray = scene->camera->GetRay(newX, newY);

		glm::vec3 planeNormal = -glm::column(scene->camera->transform->GetModelMatrix(), 2);
		glm::vec3 initialPlanePosition = firstRay.origin + (glm::dot(transform->position - firstRay.origin, planeNormal) / glm::dot(firstRay.direction, planeNormal)) * firstRay.direction;
		glm::vec3 mousePlanePosition   = ray.origin + (glm::dot(transform->position - ray.origin, planeNormal) / glm::dot(ray.direction, planeNormal)) * ray.direction;

		if (axis == TransformAxis::X) {
			glm::vec2 A = glm::normalize(glm::vec2(initialPlanePosition.z, initialPlanePosition.y));
			
			glm::vec2 biNormalA = glm::vec2(-A.y, A.x) ;
			glm::vec2 B = glm::normalize(glm::vec2(mousePlanePosition.z, mousePlanePosition.y));
			
			float displacement;
			if(glm::dot(biNormalA, B) < 0)
			{
				displacement = 57.2957795131 * glm::acos(glm::dot(A, B)); 
			}
			else
			{
				displacement = 360.0f - 57.2957795131 * glm::acos(glm::dot(A, B)); 
			}			
			for(int i=0; i<currentObjects.size(); i++) {
				currentObjects[i]->transform->rotation = firstRotations[i] + glm::vec3(1, 0, 0) * displacement; 
				currentObjects[i]->transform->hasChanged = true;
			}
		}else if (axis == TransformAxis::Y) {
			glm::vec2 A = glm::normalize(glm::vec2(initialPlanePosition.x, initialPlanePosition.z));
			
			glm::vec2 biNormalA = glm::vec2(-A.y, A.x) ;
			glm::vec2 B = glm::normalize(glm::vec2(mousePlanePosition.x, mousePlanePosition.z));
			
			float displacement;
			if(glm::dot(biNormalA, B) < 0)
			{
				displacement = 57.2957795131 * glm::acos(glm::dot(A, B)); 
			}
			else
			{
				displacement = 360.0f - 57.2957795131 * glm::acos(glm::dot(A, B)); 
			}			
	
			for(int i=0; i<currentObjects.size(); i++) {
				currentObjects[i]->transform->rotation = firstRotations[i] + glm::vec3(0, 1, 0) * displacement; 
				currentObjects[i]->transform->hasChanged = true;
			}
		} else  if (axis == TransformAxis::Z) {
			glm::vec2 A = glm::normalize(glm::vec2(initialPlanePosition));
			
			glm::vec2 biNormalA = glm::vec2(-A.y, A.x) ;
			glm::vec2 B = glm::normalize(glm::vec2(mousePlanePosition));
			
			float displacement;
			if(glm::dot(biNormalA, B) >= 0)
			{
				displacement = 57.2957795131 * glm::acos(glm::dot(A, B)); 
			}
			else
			{
				displacement = 360.0f - 57.2957795131 * glm::acos(glm::dot(A, B)); 
			}		

			for(int i=0; i<currentObjects.size(); i++) {
				currentObjects[i]->transform->rotation = firstRotations[i] + glm::vec3(0, 0, 1) * displacement; 
				currentObjects[i]->transform->hasChanged = true;
			}
		}
	}

	void TransformWidget::HandleScale(QMouseEvent* e) {
		int newX = e->x();
		int newY = e->y();


		Geometry::Ray ray = scene->camera->GetRay(newX, newY);
		double dotX = std::abs(glm::dot(ray.direction, glm::vec3(1, 0, 0)));
		double dotY = std::abs(glm::dot(ray.direction, glm::vec3(0, 1, 0)));
		double dotZ = std::abs(glm::dot(ray.direction, glm::vec3(0, 0, 1)));
		

		if (axis == TransformAxis::X) {
			double ysign = transform->position.y > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::vec4 plane = (dotY > dotZ) ? glm::vec4(0, ysign * 1, 0, std::abs(transform->position.y)) : glm::vec4(0, 0, zsign * 1, std::abs(transform->position.z));
			double t = -glm::dot(plane, glm::vec4(ray.origin, 1.0)) / glm::dot(plane, glm::vec4(ray.direction, 0.0));

			glm::vec3 position = ray.origin + t * ray.direction;
			if (isFirstFrame) { //First frame of transformation, get offset btw click and origin of transform widget
				isFirstFrame = false;
				prevX = position.x;
			}
			else {
				double newX = position.x;
				double diff = newX - prevX;
				prevX = newX;
				for(int i=0; i<currentObjects.size(); i++) {
					currentObjects[i]->transform->scale.x += diff;
					currentObjects[i]->transform->hasChanged = true;
				}				
				
			}
		} else if (axis == TransformAxis::Y) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::vec4 plane = (dotX > dotZ) ? glm::vec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::vec4(0, 0, zsign * 1, std::abs(transform->position.z));

			double t = -glm::dot(plane, glm::vec4(ray.origin, 1.0)) / glm::dot(plane, glm::vec4(ray.direction, 0.0));
			glm::vec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;
				prevY = position.y;
			}
			else {
				double newY = position.y;
				double diff = newY - prevY;
				prevY = newY;
				for(int i=0; i<currentObjects.size(); i++) {
					currentObjects[i]->transform->scale.y += diff;
					currentObjects[i]->transform->hasChanged = true;
				}								
			}
		} else if (axis == TransformAxis::Z) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double ysign = transform->position.y > 0 ? -1 : 1;
			glm::vec4 plane = (dotX > dotY) ? glm::vec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::vec4(0, ysign * 1, 0, std::abs(transform->position.y));

			double t = -glm::dot(plane, glm::vec4(ray.origin, 1.0)) / glm::dot(plane, glm::vec4(ray.direction, 0.0));
			glm::vec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;
				prevZ= position.z;
			}
			else {
				double newZ = position.z;
				double diff = newZ - prevZ;
				prevZ = newZ;
				for(int i=0; i<currentObjects.size(); i++) {
					currentObjects[i]->transform->scale.z += diff;
					currentObjects[i]->transform->hasChanged = true;
				}								
			}
		}
	}

	void TransformWidget::OnMouseReleaseEvent(QMouseEvent* e) {
		if(isTransforming) { 
			isTransforming = false;
			isFirstFrame = false;
			transformOffset = 0;
		}
	}

	Object3D* TransformWidget::Intersects(Geometry::Ray ray, double& _distance) {
		Object3D* res = nullptr;
		if(transformMode == TransformMode::TRANSLATE ) res = translateObject->Intersects(ray, _distance);
		else if(transformMode == TransformMode::SCALE) res = scaleObject->Intersects(ray, _distance);
		else if(transformMode == TransformMode::ROTATE) {
			double minDistance = 99999999999;
			int intersectInx = -1;
			for(int i=0; i<rotateObject->childObjects.size(); i++) {
				double currentDistance;
				bool intersects = Util::RayWireCircleTest(ray.origin, ray.direction, rotateObject->childObjects[i]->transform->GetWorldModelMatrix(), 1, currentDistance);
				if(intersects) {
					if(currentDistance <= minDistance) {
						minDistance = currentDistance;
						intersectInx = i;
					}
				}
			}
			if(intersectInx >= 0) res = rotateObject->childObjects[intersectInx];
		}

		if (res != nullptr) {
			firstRay = ray;
			for(int i=0; i<currentObjects.size(); i++) {
				firstRotations.push_back(this->currentObjects[i]->transform->rotation);
			}
			this->StartTransform(res);
		}

		return res;
	}

	void TransformWidget::SetTransformMode(TransformMode transformMode) {
		this->transformMode = transformMode;

		ClearObjects();

		if (transformMode == TransformMode::ROTATE) {
			AddObject(rotateObject);
		} else if (transformMode == TransformMode::SCALE) {
			AddObject(scaleObject);
		} else if (transformMode == TransformMode::TRANSLATE) {
			AddObject(translateObject);
		}
	}

}
}