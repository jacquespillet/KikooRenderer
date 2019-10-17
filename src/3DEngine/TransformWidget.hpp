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

	// Object3D* currentObject;
	std::vector<Object3D*> currentObjects;
	
	bool isTransforming = false;
	bool isFirstFrame = false;
	double transformOffset;
	TransformAxis axis;
	TransformMode transformMode;

	void Disable();

	void StartTransform(Object3D* object);

	Object3D* Intersects(Geometry::Ray ray, double& _distance);
	void Update() override;

	void OnMouseMoveEvent(QMouseEvent* e);
	void OnMouseReleaseEvent(QMouseEvent* e);

	void SetTransformMode(TransformMode transformMode);

	void HandleTranslate(QMouseEvent* e);
	void HandleRotate(QMouseEvent* e);
	void HandleScale(QMouseEvent* e);

	void RecomputePosition();

	void AddHandleObject(Object3D* object);
	void RemoveHandleObject(Object3D* object);

	double prevX = 0;
	double prevY = 0;
	double prevZ = 0;

	Geometry::Ray firstRay;
	std::vector<glm::vec3> firstRotations;
}; 

}
}