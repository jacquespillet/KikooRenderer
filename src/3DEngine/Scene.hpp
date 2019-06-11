#pragma once

#include "Common/Common.h"
#include "Components/Component.hpp"
#include "Object3D.hpp"
#include "StandardShaders.hpp"
#include "CameraScene.hpp"
#include "Components/MeshFilterComponent.hpp"

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
		virtual void OnMousePressEvent(QMouseEvent *e){}
		virtual void OnMouseReleaseEvent(QMouseEvent *e){}
		virtual void OnMouseMoveEvent(QMouseEvent *e){}
		virtual void OnWheelEvent(QWheelEvent *event){}       

		void SetWindowSize(int w, int h);

		int windowHeight;
		int windowWidth;

		bool triggerRefresh = true;

};



}
}