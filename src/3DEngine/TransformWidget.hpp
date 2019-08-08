#pragma once 
#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"


namespace KikooRenderer {
namespace CoreEngine {

class TransformWidget : public Object3D {

public: 
	enum TRANSFORM_MODE { TRANSLATE, ROTATE, SCALE };
	enum TRANSFORM_AXIS { X, Y, Z };

	TransformWidget(Scene* scene);
	Object3D* translateObject;
	Object3D* scaleObject;
	Object3D* rotateObject;
	
	double transformOffset;
	TRANSFORM_MODE transformMode = TRANSFORM_MODE::TRANSLATE;
	TRANSFORM_AXIS transformAxis;

}; 

}
}