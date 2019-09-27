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

			Object3D* xcone = GetCone(scene, "coneX", glm::dvec3(1, 0, 0), glm::dvec3(0, 90, 0), glm::dvec3(1), glm::dvec4(1.0, 0.0, 0.0, 1.0), true);
			Object3D* xline = GetLine(scene, "lineX", glm::dvec3(0, 0, 0), glm::dvec3(1, 0, 0), glm::dvec4(1.0, 0.0, 0.0, 1.0), true);
			xcone->depthTest = false;
			xline->depthTest = false;

			translateObject->AddObject(xcone);
			translateObject->AddObject(xline);

			Object3D* ycone = GetCone(scene, "coneY", glm::dvec3(0, 1, 0), glm::dvec3(-90, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 1.0, 0.0, 1.0), true);
			Object3D* yline = GetLine(scene, "lineY", glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), glm::dvec4(0.0, 1.0, 0.0, 1.0), true);
			ycone->depthTest = false;
			yline->depthTest = false;

			translateObject->AddObject(ycone);
			translateObject->AddObject(yline);

			Object3D* zcone = GetCone(scene, "coneZ", glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 0.0, 1.0, 1.0), true);
			Object3D* zline = GetLine(scene, "lineZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec4(0.0, 0.0, 1.0, 1.0), true);
			zcone->depthTest = false;
			zline->depthTest = false;

			translateObject->AddObject(zcone);
			translateObject->AddObject(zline);

			translateObject->transform->position = glm::dvec3(0);
			translateObject->transform->rotation = glm::dvec3(0);
			translateObject->transform->scale = glm::dvec3(1);
			translateObject->transform->isScreenSize = true;

			AddObject(translateObject);
		}

		{
			scaleObject = new Object3D("ScaleWidget", scene);

			Object3D* xcube = GetCube(scene, "cubeX", glm::dvec3(1, 0, 0), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(1.0, 0.0, 0.0, 1.0));
			Object3D* xline = GetLine(scene, "lineX", glm::dvec3(0, 0, 0), glm::dvec3(1, 0, 0), glm::dvec4(1.0, 0.0, 0.0, 1.0));
			xcube->depthTest = false;
			xline->depthTest = false;

			scaleObject->AddObject(xcube);
			scaleObject->AddObject(xline);

			Object3D* ycube = GetCube(scene, "cubeY", glm::dvec3(0, 1, 0), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(0.0, 1.0, 0.0, 1.0));
			Object3D* yline = GetLine(scene, "lineY", glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), glm::dvec4(0.0, 1.0, 0.0, 1.0));
			ycube->depthTest = false;
			yline->depthTest = false;

			scaleObject->AddObject(ycube);
			scaleObject->AddObject(yline);

			Object3D* zcube = GetCube(scene, "cubeZ", glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(0.0, 0.0, 1.0, 1.0));
			Object3D* zline = GetLine(scene, "lineZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec4(0.0, 0.0, 1.0, 1.0));
			zcube->depthTest = false;
			zline->depthTest = false;

			scaleObject->AddObject(zcube);
			scaleObject->AddObject(zline);

			scaleObject->transform->position = glm::dvec3(0);
			scaleObject->transform->rotation = glm::dvec3(0);
			scaleObject->transform->scale = glm::dvec3(1);
		}

		{
			rotateObject = new Object3D("RotationWidget", scene);

			Object3D* xCircle = GetWireCircle(scene, "CircleX", glm::dvec3(0, 0, 0), glm::dvec3(90, 0, 0), glm::dvec3(1), glm::dvec4(1.0, 0.0, 0.0, 1.0));
			xCircle->depthTest = false;
			rotateObject->AddObject(xCircle);

			Object3D* yCircle = GetWireCircle(scene, "CircleY", glm::dvec3(0, 0, 0), glm::dvec3(0, 90, 0), glm::dvec3(1), glm::dvec4(0.0, 1.0, 0.0, 1.0));
			yCircle->depthTest = false;
			rotateObject->AddObject(yCircle);

			Object3D* zCircle = GetWireCircle(scene, "CircleZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 0.0, 1.0, 1.0));
			zCircle->depthTest = false;
			rotateObject->AddObject(zCircle);
		}

		this->visible = false;
	}

	void TransformWidget::SetObject(Object3D* object) {
		currentObject = object;
		TransformComponent* objectTransform = object->transform;
		transform->position = objectTransform->GetWorldPosition();
		visible = true;
	}

	void TransformWidget::Disable() {
		currentObject = nullptr;
		visible = false;
	}

	void TransformWidget::StartTransform(Object3D* object) {
		visible = true;
		isTransforming = true;
		isFirstFrame = true;
		if(object->name == "coneX" || object->name == "cubeX") axis = TransformAxis::X;
		else if (object->name == "coneY" || object->name == "cubeY") axis = TransformAxis::Y;
		else if (object->name == "coneZ" || object->name == "cubeZ") axis = TransformAxis::Z;
	}

	void TransformWidget::OnMouseMoveEvent(QMouseEvent* e) {
		if (isTransforming) {
			TransformComponent* objectTransform = currentObject->transform;
			if(objectTransform != nullptr) {
				if(transformMode == TransformMode::TRANSLATE) {
					HandleTranslate(e, objectTransform);
				} else if (transformMode == TransformMode::SCALE) {
					HandleScale(e, objectTransform);
				} else if (transformMode == TransformMode::ROTATE) {
					HandleRotate(e, objectTransform);
				}
				scene->objectDetailsPanel->Refresh();
			}
		}
	}

	void TransformWidget::HandleTranslate(QMouseEvent* e, TransformComponent* objectTransform) {
		int newX = e->x();
		int newY = e->y();

		
		Geometry::Ray ray = scene->camera->GetRay(newX, newY);
		double dotX = std::abs(glm::dot(ray.direction, glm::dvec3(1, 0, 0)));
		double dotY = std::abs(glm::dot(ray.direction, glm::dvec3(0, 1, 0)));
		double dotZ = std::abs(glm::dot(ray.direction, glm::dvec3(0, 0, 1)));

		if (axis == TransformAxis::X) {
			double ysign = transform->position.y > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::dvec4 plane = (dotY > dotZ) ? glm::dvec4(0, ysign * 1, 0, std::abs(transform->position.y)) : glm::dvec4(0, 0, zsign * 1, std::abs(transform->position.z));
			double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));

			glm::dvec3 position = ray.origin + t * ray.direction;
			if (isFirstFrame) { //First frame of transformation, get offset btw click and origin of transform widget
				isFirstFrame = false;
				//Get position of ray on the plane
				transformOffset = position.x - transform->position.x;
			}
			else {
				//Set transform to x position of intersection
				double newX = position.x - transformOffset;
				transform->position.x = newX;
				double xScaleFactor = objectTransform->GetWorldScale().x;
				objectTransform->SetWorldX(newX);
			}
		}
		else if (axis == TransformAxis::Y) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::dvec4 plane = (dotX > dotZ) ? glm::dvec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::dvec4(0, 0, zsign * 1, std::abs(transform->position.z));

			double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
			glm::dvec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;
				transformOffset = position.y - transform->position.y;
			}
			else {
				double newY = position.y - transformOffset;
				transform->position.y = newY;
				objectTransform->SetWorldY(newY);
			}
		}
		else if (axis == TransformAxis::Z) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double ysign = transform->position.y > 0 ? -1 : 1;
			glm::dvec4 plane = (dotX > dotY) ? glm::dvec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::dvec4(0, ysign * 1, 0, std::abs(transform->position.y));

			double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
			glm::dvec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;

				transformOffset = position.z - transform->position.z;
			}
			else {
				double newZ = position.z - transformOffset;
				transform->position.z = newZ;
				objectTransform->SetWorldZ(newZ);
			}
		}
	}

	void TransformWidget::HandleRotate(QMouseEvent* e, TransformComponent* objectTransform) {

	}

	void TransformWidget::HandleScale(QMouseEvent* e, TransformComponent* objectTransform) {
		int newX = e->x();
		int newY = e->y();


		Geometry::Ray ray = scene->camera->GetRay(newX, newY);
		double dotX = std::abs(glm::dot(ray.direction, glm::dvec3(1, 0, 0)));
		double dotY = std::abs(glm::dot(ray.direction, glm::dvec3(0, 1, 0)));
		double dotZ = std::abs(glm::dot(ray.direction, glm::dvec3(0, 0, 1)));
		

		if (axis == TransformAxis::X) {
			double ysign = transform->position.y > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::dvec4 plane = (dotY > dotZ) ? glm::dvec4(0, ysign * 1, 0, std::abs(transform->position.y)) : glm::dvec4(0, 0, zsign * 1, std::abs(transform->position.z));
			double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));

			glm::dvec3 position = ray.origin + t * ray.direction;
			if (isFirstFrame) { //First frame of transformation, get offset btw click and origin of transform widget
				isFirstFrame = false;
				prevX = position.x;
			}
			else {
				double newX = position.x;
				double diff = newX - prevX;
				prevX = newX;
				objectTransform->scale.x += diff;
			}
		} else if (axis == TransformAxis::Y) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double zsign = transform->position.z > 0 ? -1 : 1;

			glm::dvec4 plane = (dotX > dotZ) ? glm::dvec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::dvec4(0, 0, zsign * 1, std::abs(transform->position.z));

			double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
			glm::dvec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;
				prevY = position.y;
			}
			else {
				double newY = position.y;
				double diff = newY - prevY;
				prevY = newY;
				objectTransform->scale.y += diff;
			}
		} else if (axis == TransformAxis::Z) {
			double xsign = transform->position.x > 0 ? -1 : 1;
			double ysign = transform->position.y > 0 ? -1 : 1;
			glm::dvec4 plane = (dotX > dotY) ? glm::dvec4(xsign * 1, 0, 0, std::abs(transform->position.x)) : glm::dvec4(0, ysign * 1, 0, std::abs(transform->position.y));

			double t = -glm::dot(plane, glm::dvec4(ray.origin, 1.0)) / glm::dot(plane, glm::dvec4(ray.direction, 0.0));
			glm::dvec3 position = ray.origin + t * ray.direction;

			if (isFirstFrame) {
				isFirstFrame = false;
				prevZ= position.z;
			}
			else {
				double newZ = position.z;
				double diff = newZ - prevZ;
				prevZ = newZ;
				objectTransform->scale.z += diff;
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
			for(int i=0; i<rotateObject->childObjects.size(); i++) {
				Util::RayWireCircleTest(ray.origin, ray.direction, rotateObject->childObjects[i]->transform->GetWorldModelMatrix(), 1);
			}
			res = rotateObject->childObjects[0];
		}

		if (res != nullptr) {
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