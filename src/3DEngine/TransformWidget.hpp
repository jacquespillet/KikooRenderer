#pragma once 
#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"


namespace KikooRenderer {
namespace CoreEngine {

class TransformWidget : public Object3D {

public:
	enum TransformMode { TRANSLATE, ROTATE, SCALE };
	enum TransformAxis { X, Y, Z };

	TransformWidget(Scene* scene);
	Object3D* translateObject;
	Object3D* scaleObject;
	Object3D* rotateObject;

	Object3D* currentObject;
	
	bool isTransforming = false;
	bool isFirstFrame = false;
	double transformOffset;
	TransformAxis axis;
	TransformMode transformMode;

	void SetObject(Object3D* object);
	void Disable();

	void StartTransform(Object3D* object);

	Object3D* Intersects(Geometry::Ray ray, double& _distance);

	void OnMouseMoveEvent(QMouseEvent* e);
	void OnMouseReleaseEvent(QMouseEvent* e);

	void SetTransformMode(TransformMode transformMode);

	void HandleTranslate(QMouseEvent* e, TransformComponent* objectTransform);
	void HandleRotate(QMouseEvent* e, TransformComponent* objectTransform);
	void HandleScale(QMouseEvent* e, TransformComponent* objectTransform);

	double prevX = 0;
	double prevY = 0;
	double prevZ = 0;

	Geometry::Ray firstRay;
	glm::vec3 firstRotation;
}; 

}
}