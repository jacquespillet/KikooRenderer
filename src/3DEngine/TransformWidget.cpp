#include "TransformWidget.hpp"

namespace KikooRenderer {
namespace CoreEngine {

	TransformWidget::TransformWidget(Scene* scene) : Object3D("TransformWidget", scene) {
		////{
		////	translateObject = new Object3D("TranslateWidget", scene);

		////	Object3D* xcone = GetCone(scene, "coneX", glm::dvec3(1, 0, 0), glm::dvec3(0, 90, 0), glm::dvec3(1), glm::dvec4(1.0, 0.0, 0.0, 1.0), true);
		////	Object3D* xline = GetLine(scene, "lineX", glm::dvec3(0, 0, 0), glm::dvec3(1, 0, 0), glm::dvec4(1.0, 0.0, 0.0, 1.0), true);
		////	xcone->depthTest = false;
		////	xline->depthTest = false;

		////	translateObject->AddObject(xcone);
		////	translateObject->AddObject(xline);

		////	Object3D* ycone = GetCone(scene, "coneY", glm::dvec3(0, 1, 0), glm::dvec3(-90, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 1.0, 0.0, 1.0), true);
		////	Object3D* yline = GetLine(scene, "lineY", glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), glm::dvec4(0.0, 1.0, 0.0, 1.0), true);
		////	ycone->depthTest = false;
		////	yline->depthTest = false;

		////	translateObject->AddObject(ycone);
		////	translateObject->AddObject(yline);

		////	Object3D* zcone = GetCone(scene, "coneZ", glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 0.0, 1.0, 1.0), true);
		////	Object3D* zline = GetLine(scene, "lineZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec4(0.0, 0.0, 1.0, 1.0), true);
		////	zcone->depthTest = false;
		////	zline->depthTest = false;

		////	translateObject->AddObject(zcone);
		////	translateObject->AddObject(zline);

		////	translateObject->transform->position = glm::dvec3(0);
		////	translateObject->transform->rotation = glm::dvec3(0);
		////	translateObject->transform->scale = glm::dvec3(1);
		////	translateObject->transform->isScreenSize = true;

		////	AddObject(translateObject);
		////}

		////{
		////	scaleObject = new Object3D("ScaleWidget", scene);

		////	Object3D* xcube = GetCube(scene, "cubeX", glm::dvec3(1, 0, 0), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(1.0, 0.0, 0.0, 1.0));
		////	Object3D* xline = GetLine(scene, "lineX", glm::dvec3(0, 0, 0), glm::dvec3(1, 0, 0), glm::dvec4(1.0, 0.0, 0.0, 1.0));
		////	xcube->depthTest = false;
		////	xline->depthTest = false;

		////	scaleObject->AddObject(xcube);
		////	scaleObject->AddObject(xline);

		////	Object3D* ycube = GetCube(scene, "cubeY", glm::dvec3(0, 1, 0), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(0.0, 1.0, 0.0, 1.0));
		////	Object3D* yline = GetLine(scene, "lineY", glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), glm::dvec4(0.0, 1.0, 0.0, 1.0));
		////	ycube->depthTest = false;
		////	yline->depthTest = false;

		////	scaleObject->AddObject(ycube);
		////	scaleObject->AddObject(yline);

		////	Object3D* zcube = GetCube(scene, "cubeZ", glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 0), glm::dvec3(0.5), glm::dvec4(0.0, 0.0, 1.0, 1.0));
		////	Object3D* zline = GetLine(scene, "lineZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 1), glm::dvec4(0.0, 0.0, 1.0, 1.0));
		////	zcube->depthTest = false;
		////	zline->depthTest = false;

		////	scaleObject->AddObject(zcube);
		////	scaleObject->AddObject(zline);

		////	scaleObject->transform->position = glm::dvec3(0);
		////	scaleObject->transform->rotation = glm::dvec3(0);
		////	scaleObject->transform->scale = glm::dvec3(1);
		////}

		////{
		////	rotateObject = new Object3D("RotationWidget", scene);

		////	Object3D* xCircle = GetWireCircle(scene, "CircleX", glm::dvec3(0, 0, 0), glm::dvec3(90, 0, 0), glm::dvec3(1), glm::dvec4(1.0, 0.0, 0.0, 1.0));
		////	xCircle->depthTest = false;
		////	rotateObject->AddObject(xCircle);

		////	Object3D* yCircle = GetWireCircle(scene, "CircleY", glm::dvec3(0, 0, 0), glm::dvec3(0, 90, 0), glm::dvec3(1), glm::dvec4(0.0, 1.0, 0.0, 1.0));
		////	yCircle->depthTest = false;
		////	rotateObject->AddObject(yCircle);

		////	Object3D* zCircle = GetWireCircle(scene, "CircleZ", glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0), glm::dvec3(1), glm::dvec4(0.0, 0.0, 1.0, 1.0));
		////	zCircle->depthTest = false;
		////	rotateObject->AddObject(zCircle);
		////}

		////this->visible = false;
	}
}
}