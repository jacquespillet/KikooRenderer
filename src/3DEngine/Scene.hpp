#pragma once

#include "Util/Common.h"
#include "Object3D.hpp"
#include "StandardShaders.hpp"
#include "CameraScene.hpp"
#include "Components/MeshFilterComponent.hpp"
//#include "TransformWidget.hpp"


namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class CameraScene;

class Scene {
    public: 
        Scene();
        void Start();
		void Render();
        void Enable();
        // void Disable();
        // void Destroy();
        bool started;

		std::vector<Object3D*> objects3D; 
		std::vector<Object3D*> lightObjects; 
		StandardShaders standardShaders;
		CameraScene camera;

		bool test = false;

		void virtual OnStart(){};
		void virtual OnUpdate();
		void virtual OnRender(){};
		void virtual OnEnable(){};
		void virtual OnDisable(){};
		void virtual OnDestroy();
		
		virtual void OnKeyPressEvent(QKeyEvent *e);
		virtual void OnKeyReleaseEvent(QKeyEvent *e){}
		virtual void OnMousePressEvent(QMouseEvent *e);
		virtual void OnMouseReleaseEvent(QMouseEvent *e);
		virtual void OnMouseMoveEvent(QMouseEvent *e);
		virtual void OnWheelEvent(QWheelEvent *event);       

		void HandleSelection(int x, int y);

		Object3D* FindObjectByName(std::string name);
		std::string AddObject(Object3D* object);
		void RemoveObject(Object3D* object);

		void SetWindowSize(int w, int h);

		int windowHeight;
		int windowWidth;

		bool triggerRefresh = true;

		clock_t previousTime=0;
		double deltaTime;

		std::vector<Object3D*> selectedObjects;

		//TransformWidget* transformWidget;
		Object3D* transformWidget;

		Object3D* scaleWidget;
		Object3D* rotationWidget;

		bool isTransforming = false;

		GLint defaultFBO;

		void AddObjectToSelection(bool erasePrevious, Object3D* selectedObject);
		void ClearSelection();


	protected:
		Object3D* GetIntersectObject(int x, int y);

		
		void TransformSelection(QMouseEvent *e);
		bool isFirstTransformFrame;
		//double transformOffset;
		//TRANSFORM_MODE transformMode = TRANSFORM_MODE::TRANSLATE;
		//TRANSFORM_AXIS transformAxis;
};

}
}