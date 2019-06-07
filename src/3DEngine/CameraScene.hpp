#pragma once

#include "Common/Common.h"
// #include "Scene.hpp"
#include "TransformComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Scene;
class CameraScene{
    public: 
        CameraScene(Scene* scene, double eyeDistance, double fov, double near, double far, double aspect);
        
        CameraScene(){
            std::cout <<"build camera" << std::endl;            
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
};

}
}