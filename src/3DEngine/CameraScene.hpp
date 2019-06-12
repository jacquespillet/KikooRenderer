#pragma once

#include "Common/Common.h"
#include "Components/TransformComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Scene;
class CameraScene{
    public: 
        CameraScene(Scene* scene, double eyeDistance, double fov, double near, double far, double aspect);
        
        CameraScene() :  transform(TransformComponent(nullptr)) {
        }
        Scene* scene;
        double eyeDistance;
        double fov;
        double nearClip;
        double farClip;
        double aspect;
        glm::mat4 projectionMatrix;
        TransformComponent transform;


        glm::dmat4 GetProjectionMatrix();
        glm::dmat4 GetViewMatrix();
        glm::dmat4 GetModelTransform();
        
        void UpdateProjectionMatrix();

        void OnKeyPressEvent(QKeyEvent *e);
		void OnMousePressEvent(QMouseEvent *e);
		void OnMouseReleaseEvent(QMouseEvent *e);
		void OnMouseMoveEvent(QMouseEvent *e);
		void OnWheelEvent(QWheelEvent *event); 

    private: 
        bool isRightClicked = false;
        bool isLeftClicked = false;
        bool isMiddleClicked = false;
        int previousX;
        int previousY;

        float speedFactor = 0.1;
};

}
}