#include "CameraScene.hpp"
#include "Scene.hpp"

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

    this->transform.position.z = -5;

}

glm::dmat4 CameraScene::GetProjectionMatrix() {
    return this->projectionMatrix;
}

glm::dmat4 CameraScene::GetViewMatrix() {
    return glm::inverse(this->transform.GetModelMatrix());  
}

glm::dmat4 CameraScene::GetModelTransform() {
    return this->transform.GetModelMatrix();
}

void CameraScene::UpdateProjectionMatrix() {
    this->projectionMatrix = glm::perspectiveLH(this->fov, this->aspect, this->nearClip, this->farClip);     
}

void CameraScene::OnKeyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Z) {
        this->transform.position.z+=0.1;
    } else if(e->key() == Qt::Key_S) {
        this->transform.position.z-=0.1;
    } else if(e->key() == Qt::Key_Q) {
        this->transform.position.x-=0.1;
    } else if(e->key() == Qt::Key_D) {
        this->transform.position.x+=0.1;
    }   else if(e->key() == Qt::Key_Up) {
        this->transform.position.y+=0.1;
    }   else if(e->key() == Qt::Key_Down) {
        this->transform.position.y-=0.1;
    }   else if(e->key() == Qt::Key_Right) {
        this->transform.rotation.y+=1.0;
    }   else if(e->key() == Qt::Key_Left) {
        this->transform.rotation.y-=1.0;
    }      
     
    scene->triggerRefresh = true;
}

}
}