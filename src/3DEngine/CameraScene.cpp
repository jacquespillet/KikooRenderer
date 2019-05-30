#include "CameraScene.hpp"

namespace KikooRenderer {
namespace CoreEngine {

CameraScene::CameraScene(Scene* scene, double eyeDistance, double fov, double near, double far){
    this->scene = scene;
    this->eyeDistance = eyeDistance;
    this->fov = fov;
    this->nearClip = nearClip;
    this->farClip = farClip;
    this->projectionMatrix =glm::perspective(this->fov, 1.0, this->nearClip, this->farClip); 
}

glm::dmat4 CameraScene::GetProjectionMatrix() {
    return this->projectionMatrix;
}

glm::dmat4 CameraScene::GetViewMatrix() {
    glm::dmat4 res = glm::dmat4(1.0);
    return res;
}

glm::dmat4 CameraScene::GetModelTransform() {
    glm::dmat4 res = glm::dmat4(1.0);
    return res;
}

}
}