#pragma once

#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Geometry/Planes.hpp"
#include "Object3D.hpp"
namespace KikooRenderer {
namespace CoreEngine {

class TransformComponent;
class Scene;
class CameraController;

class CameraScene : public Object3D{
    public: 
		enum ProjectionType {
			Perspective,
			Orthographic
		};

        CameraScene(Scene* scene, float eyeDistance, float fov, float near, float far, float aspect);
        
		CameraScene();



        glm::mat4 GetProjectionMatrix();
        glm::mat4 GetViewMatrix();
        glm::mat4 GetModelTransform();
        glm::vec3 GetPosition();
        
        void UpdateProjectionMatrix();

        void OnKeyPressEvent(QKeyEvent *e);
		void OnMousePressEvent(QMouseEvent *e);
		void OnMouseReleaseEvent(QMouseEvent *e);
		void OnMouseMoveEvent(QMouseEvent *e);
		void OnWheelEvent(QWheelEvent *event);
		void OnKeyReleaseEvent(QKeyEvent *e);
		void OnUpdate();

        Geometry::Ray GetRay(int x, int y);
        Geometry::Ray GetRay(double x, double y);
		Geometry::Planes GetPlanes();

        float GetScreenScale(glm::vec3 objPos);

        //Accessors
        float GetEyeDistance();
        void  SetEyeDistance(float value);
        float GetFov();
        void  SetFov(float value);
        float GetNearClip();
        void  SetNearClip(float value);
        float GetFarClip();
        void  SetFarClip(float value);
        float GetAspect();
        void  SetAspect(float value);
        void  SetProjectionMatrix(glm::mat4 value);
        void  SetViewMatrix(glm::mat4 value);
        glm::mat4 GetPreviousViewMatrix();
        void  SetPreviousViewMatrix(glm::mat4 value);
        CameraController* GetCameraController();
        void  SetCameraController(CameraController* value);
        float GetOrthoFOV();
        void  SetOrthoFOV(float value);
        float GetSpeedFactor();
        void  SetSpeedFactor(float value);

        bool sortObjects = true;

    protected:
        float eyeDistance;
        float fov;
        float nearClip;
        float farClip;
        float aspect;
        glm::mat4 projectionMatrix;

		ProjectionType projectionType;
        glm::mat4 viewMatrix;
        glm::mat4 previousViewMatrix;

        CameraController* cameraController;

        float orthoFOV = 10.0;
        float speedFactor = 0.1;
};

}
}