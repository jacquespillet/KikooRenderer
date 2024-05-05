#include "CameraController.hpp"
#include "../CameraScene.hpp"
#include "../Scene.hpp"
#include "../Components/TransformComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

CameraController::CameraController(CameraScene* camera) {
    this->camera = camera;
}

void CameraController::OnKeyReleaseEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_W && !e->isAutoRepeat()) {
        forwardPressed = false;
    }  
    if(e->key() == Qt::Key_S && !e->isAutoRepeat()) {
        backwardPressed = false;
    } 
    if(e->key() == Qt::Key_D && !e->isAutoRepeat()) {
        rightPressed = false;
    } 
    if(e->key() == Qt::Key_A && !e->isAutoRepeat()) {
        leftPressed = false;
    }

    if(!leftPressed && !rightPressed && !forwardPressed && !backwardPressed) {
        elapsedTimeMoving = 0;
    }
}
void CameraController::OnUpdate() {
    glm::vec3 forwardVec = glm::column(this->camera->transform->GetModelMatrix(), 2);
    glm::vec3 rightVec = glm::column(this->camera->transform->GetModelMatrix(), 0);
    if(forwardPressed) {
        this->camera->transform->position += elapsedTimeMoving * forwardVec * speedFactor * (float)this->camera->scene->deltaTime;
        camera->scene->triggerRefresh = true;
        elapsedTimeMoving = (std::min)((float)(elapsedTimeMoving + (float)this->camera->scene->deltaTime), 4.0f);
        
        this->camera->transform->hasChanged = true;
    }

    if(backwardPressed) {
        this->camera->transform->position -= elapsedTimeMoving * forwardVec * speedFactor * (float)this->camera->scene->deltaTime;
        camera->scene->triggerRefresh = true;
        elapsedTimeMoving = (std::min)((float)(elapsedTimeMoving + (float)this->camera->scene->deltaTime), 4.0f);
        
        this->camera->transform->hasChanged = true;
    }

    if(rightPressed) {
        this->camera->transform->position += elapsedTimeMoving * rightVec * speedFactor * (float)this->camera->scene->deltaTime;
        camera->scene->triggerRefresh = true;
        elapsedTimeMoving = (std::min)((float)(elapsedTimeMoving + (float)this->camera->scene->deltaTime), 4.0f);
        
        this->camera->transform->hasChanged = true;
    }

    
    if(leftPressed) {
        this->camera->transform->position -= elapsedTimeMoving * rightVec * speedFactor * (float)this->camera->scene->deltaTime;
        camera->scene->triggerRefresh = true;
        elapsedTimeMoving = (std::min)((float)(elapsedTimeMoving + (float)this->camera->scene->deltaTime), 4.0f);
        
        this->camera->transform->hasChanged = true;
    }
}

void CameraController::MoveToFocus() {
    if(this->camera->scene->selectedObjects.size() > 0) {
        this->camera->transform->StartAnimate(this->camera->scene->transformWidget->transform->position, glm::vec3(0), glm::vec3(1));
    }
}


void CameraController::OnKeyPressEvent(QKeyEvent *e) {
    if(isRightClicked) {
        if(e->key() == Qt::Key_W) {
            forwardPressed = true;
        }  
        if(e->key() == Qt::Key_S) {
            backwardPressed = true;
        } 
        if(e->key() == Qt::Key_D) {
            rightPressed = true;
        } 
        if(e->key() == Qt::Key_A) {
            leftPressed = true;
        }
        camera->scene->triggerRefresh = true;
    }
}

void CameraController::OnMousePressEvent(QMouseEvent *e) {
    if(e->button() == Qt::LeftButton) {
        isLeftClicked = true;
    }
    
    if(e->button() == Qt::RightButton) {
        isRightClicked = true;
    }

    if(e->button() == Qt::MidButton) {
        isMiddleClicked = true;
    }

    previousX = e->x();
    previousY = e->y();

}

void CameraController::OnMouseReleaseEvent(QMouseEvent *e) {
    if(isLeftClicked) isLeftClicked = false;
    if(isRightClicked) isRightClicked = false;
    if(isMiddleClicked) isMiddleClicked = false;
}

void CameraController::OnMouseMoveEvent(QMouseEvent *e) {
    if(isRightClicked) {
        int newX = e->x();
        int newY = e->y();

        int xOffset = newX - previousX;
        int yOffset = newY - previousY;

        this->camera->transform->rotation.y += (float)xOffset * 0.1;
        this->camera->transform->rotation.x += (float)yOffset * 0.1;

        previousX = newX;
        previousY = newY;
        
        this->camera->transform->hasChanged = true;
    }

    if(isMiddleClicked) {
        int newX = e->x();
        int newY = e->y();

        int xOffset = newX - previousX;
        int yOffset = newY - previousY;

        glm::mat4 transform = this->camera->transform->GetModelMatrix();

        this->camera->transform->position.x -= glm::column(transform, 0).x * xOffset * speedFactor * 0.001;
        this->camera->transform->position.y -= glm::column(transform, 0).y * xOffset * speedFactor * 0.001;
        this->camera->transform->position.z -= glm::column(transform, 0).z * xOffset * speedFactor * 0.001;
        
        this->camera->transform->position.x += glm::column(transform, 1).x * yOffset * speedFactor * 0.001;
        this->camera->transform->position.y += glm::column(transform, 1).y * yOffset * speedFactor * 0.001;
        this->camera->transform->position.z += glm::column(transform, 1).z * yOffset * speedFactor * 0.001;

        previousX = newX;
        previousY = newY;
        
        this->camera->transform->hasChanged = true;
    }
}

void CameraController::OnWheelEvent(QWheelEvent *e) {
    QPoint point = e->angleDelta();

    glm::mat4 transform = this->camera->transform->GetModelMatrix();
    if(point.y() > 0) {
        this->camera->transform->position.x += glm::column(transform, 2).x * speedFactor * 0.1;
        this->camera->transform->position.y += glm::column(transform, 2).y * speedFactor * 0.1;
        this->camera->transform->position.z += glm::column(transform, 2).z * speedFactor * 0.1;
        this->camera->SetOrthoFOV(0.5);
        
        this->camera->transform->hasChanged = true;
    } else if(point.y() < 0) {
        this->camera->transform->position.x -= glm::column(transform, 2).x * speedFactor * 0.1;
        this->camera->transform->position.y -= glm::column(transform, 2).y * speedFactor * 0.1;
        this->camera->transform->position.z -= glm::column(transform, 2).z * speedFactor * 0.1;
        this->camera->SetOrthoFOV(0.5);
        
        this->camera->transform->hasChanged = true;
    }
}

}
}