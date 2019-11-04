#pragma once

#include "Util/Common.h"
#include "StandardShaders.hpp"
#include "CameraScene.hpp"
#include "Object3D.hpp"
#include "TransformWidget.hpp"
#include "ObjectDetails/ObjectDetails.hpp"
#include "Cubemap.hpp"
#include "Renderers/HDRRenderer.hpp"
#include "DrawImmediate.hpp"

#include "PhysicsEngines/Physics.hpp"

namespace KikooRenderer {
class ObjectDetailsPanel;
class SceneTree;

namespace CoreEngine {

class Renderer;

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
		std::vector<Object3D*> rayMarchedObjects;
		// std::vector<ParticleSystem*> particleSystems;
		ParticleSystem* ps;

		StandardShaders standardShaders;
		CameraScene* camera;

		DrawImmediate drawImmediate;

		bool test = false;

		void virtual OnUpdate();
		void virtual OnDestroy();
		
		virtual void OnKeyPressEvent(QKeyEvent *e);
		virtual void OnKeyReleaseEvent(QKeyEvent *e);
		virtual void OnMousePressEvent(QMouseEvent *e);
		virtual void OnMouseReleaseEvent(QMouseEvent *e);
		virtual void OnMouseMoveEvent(QMouseEvent *e);
		virtual void OnWheelEvent(QWheelEvent *event);       
		virtual void OnMouseDoubleClickEvent( QMouseEvent * e );


		Object3D* FindObjectByName(std::string name);
		std::string AddObject(Object3D* object);
		void RemoveObject(Object3D* object);

		void GetObjectsInSquare(glm::vec2 topLeft, glm::vec2 bottomRight);
		void HandleSelection(int x, int y, bool isCtrl);
		void AddObjectToSelection(bool erasePrevious, Object3D* selectedObject);
		void ClearSelection();
		void DeleteSelection();


		void SetWindowSize(int w, int h);

		int windowHeight;
		int windowWidth;

		bool triggerRefresh = true;

		clock_t previousTime=0;
		double deltaTime;
		double elapsedTime = 0.0;

		std::vector<Object3D*> selectedObjects;
		
		//UI Elements
		bool rendersUI = true;
		Object3D* axes;
		Object3D* axesHelper;
		Object3D* grid;
		TransformWidget* transformWidget;
		bool isTransforming = false;

		GLint defaultFBO; 

		bool isControlKey = false;

		//Right panel inspector
		KikooRenderer::ObjectDetailsPanel* objectDetailsPanel;
		
		
		//Scene skybox
		bool hasSkybox = false;
		void SetSkybox(std::vector<std::string> filenames);
		Object3D* skyboxCube;
		
		//For saving the scene
		QJsonObject ToJSON();
		void FromJSON(QJsonDocument json);


		void SetRenderPipeline(RENDER_PIPELINE pipeline);
		Renderer* renderer;

		QOpenGLWidget* glWindow; //Useful for catching GL context

		SceneTree* sceneTree;

		bool renderGrid = true;
		bool renderAxes= true;
		bool renderAxesHelper= true;
		bool renderTransformWidget= true;

		void SetLayerMask(uint16_t _layerMask);
		uint16_t GetLayerMask();

		Physics::Bullet::Simulation* GetSimulation();
  
		void PlayPause();
		void Stop();
		bool IsPlaying();

	protected:
		bool isPlaying = false;
		float animationFrametime;

		Object3D* GetIntersectObject(int x, int y);
		uint16_t layerMask = 0xFFFF;

		bool isEditMode = false;
		Object3D* editedObject;

		Physics::Bullet::Simulation simulation;
};

}
}