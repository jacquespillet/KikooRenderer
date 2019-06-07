#include "CameraScene.hpp"

namespace KikooRenderer {
namespace CoreEngine {

CameraScene::CameraScene(Scene* _scene, double _eyeDistance, double _fov, double _near, double _far, double _aspect){
    this->scene = _scene;
    this->eyeDistance = _eyeDistance;
    this->fov = _fov;
    this->nearClip = _near;
    this->farClip = _far;
    this->aspect = _aspect;

    UpdateProjectionMatrix();

}

glm::dmat4 CameraScene::GetProjectionMatrix() {
    return this->projectionMatrix;
}

glm::dmat4 CameraScene::GetViewMatrix() {
    glm::dmat4 res = glm::dmat4(1.0);
    return res;
}

glm::dmat4 CameraScene::GetModelTransform() {
    return this->transform.GetModelMatrix();
}

void CameraScene::UpdateProjectionMatrix() {
    this->projectionMatrix = glm::perspective(this->fov, this->aspect, this->nearClip, this->farClip);     
}

}
}