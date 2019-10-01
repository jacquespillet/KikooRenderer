#include "CameraScene.hpp"
#include "Components/TransformComponent.hpp"
#include "Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {

CameraScene::CameraScene(Scene* _scene, float _eyeDistance, float _fov, float _near, float _far, float _aspect) {
	transform = new TransformComponent(nullptr);

    this->scene = _scene;
    this->eyeDistance = _eyeDistance;
    this->fov = _fov;
    this->nearClip = _near;
    this->farClip = _far;
    this->aspect = _aspect;

    UpdateProjectionMatrix();

    this->transform->position.x = 0;
    this->transform->position.y = 10;
    this->transform->position.z = -10;

	projectionType = ProjectionType::Perspective;
    viewMatrix = glm::inverse(this->transform->GetModelMatrix()); 

    orthoFOV = 10;
}

glm::vec3 CameraScene::GetPosition() {
    return this->transform->position;
}

glm::mat4 CameraScene::GetProjectionMatrix() {
    return this->projectionMatrix;
}

glm::mat4 CameraScene::GetViewMatrix() {
    return glm::inverse(this->transform->GetModelMatrix());
}

glm::mat4 CameraScene::GetModelTransform() {
    return this->transform->GetModelMatrix();
}

void CameraScene::UpdateProjectionMatrix() {
	if(projectionType == ProjectionType::Perspective)  this->projectionMatrix = glm::perspectiveLH(this->fov, this->aspect, this->nearClip, this->farClip);     
	else this->projectionMatrix = glm::orthoLH(-orthoFOV * this->aspect, orthoFOV * this->aspect, -orthoFOV, orthoFOV, 0.0001f, this->farClip);
}

void CameraScene::OnKeyPressEvent(QKeyEvent *e){
    if(isRightClicked) {
        glm::mat4 transform = this->transform->GetModelMatrix();

        if(e->key() == Qt::Key_Z) {
            this->transform->position.x += glm::column(transform, 2).x * speedFactor;
            this->transform->position.y += glm::column(transform, 2).y * speedFactor;
            this->transform->position.z += glm::column(transform, 2).z * speedFactor;
        }  
        if(e->key() == Qt::Key_S) {
            this->transform->position.x -= glm::column(transform, 2).x * speedFactor;
            this->transform->position.y -= glm::column(transform, 2).y * speedFactor;
            this->transform->position.z -= glm::column(transform, 2).z * speedFactor;
        } 
        if(e->key() == Qt::Key_Q) {
            this->transform->position.x -= glm::column(transform, 0).x * speedFactor;
            this->transform->position.y -= glm::column(transform, 0).y * speedFactor;
            this->transform->position.z -= glm::column(transform, 0).z * speedFactor;
        } 
        if(e->key() == Qt::Key_D) {
            this->transform->position.x += glm::column(transform, 0).x * speedFactor;
            this->transform->position.y += glm::column(transform, 0).y * speedFactor;
            this->transform->position.z += glm::column(transform, 0).z * speedFactor;
        }

        previousViewMatrix = glm::mat4(viewMatrix);
        viewMatrix = glm::inverse(this->transform->GetModelMatrix());
    }  

	if (e->key() == Qt::Key_5) {
		if (projectionType == ProjectionType::Perspective) projectionType = ProjectionType::Orthographic;
		else projectionType = ProjectionType::Perspective;
			
		UpdateProjectionMatrix();

	}
}

void CameraScene::OnMousePressEvent(QMouseEvent *e) {
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
void CameraScene::OnMouseReleaseEvent(QMouseEvent *e) {
    if(isLeftClicked) isLeftClicked = false;
    if(isRightClicked) isRightClicked = false;
    if(isMiddleClicked) isMiddleClicked = false;
}
void CameraScene::OnMouseMoveEvent(QMouseEvent *e) {
    if(isLeftClicked) {
    }
    if(isRightClicked) {
        int newX = e->x();
        int newY = e->y();

        int xOffset = newX - previousX;
        int yOffset = newY - previousY;

        this->transform->rotation.y += (float)xOffset * 0.1;
        this->transform->rotation.x += (float)yOffset * 0.1;

        previousX = newX;
        previousY = newY;
    }

    if(isMiddleClicked) {
        int newX = e->x();
        int newY = e->y();

        int xOffset = newX - previousX;
        int yOffset = newY - previousY;

        glm::mat4 transform = this->transform->GetModelMatrix();

        this->transform->position.x -= glm::column(transform, 0).x * xOffset * speedFactor * 0.1;
        this->transform->position.y -= glm::column(transform, 0).y * xOffset * speedFactor * 0.1;
        this->transform->position.z -= glm::column(transform, 0).z * xOffset * speedFactor * 0.1;
        
        this->transform->position.x += glm::column(transform, 1).x * yOffset * speedFactor * 0.1;
        this->transform->position.y += glm::column(transform, 1).y * yOffset * speedFactor * 0.1;
        this->transform->position.z += glm::column(transform, 1).z * yOffset * speedFactor * 0.1;

        previousX = newX;
        previousY = newY;        
    }
    previousViewMatrix = glm::mat4(viewMatrix);
    viewMatrix = glm::inverse(this->transform->GetModelMatrix());
}
void CameraScene::OnWheelEvent(QWheelEvent *event) {
    QPoint point = event->angleDelta();

    glm::mat4 transform = this->transform->GetModelMatrix();
    if(point.y() > 0) {
        this->transform->position.x += glm::column(transform, 2).x * speedFactor;
        this->transform->position.y += glm::column(transform, 2).y * speedFactor;
        this->transform->position.z += glm::column(transform, 2).z * speedFactor;
        orthoFOV -= 0.5;
    } else if(point.y() < 0) {
        this->transform->position.x -= glm::column(transform, 2).x * speedFactor;
        this->transform->position.y -= glm::column(transform, 2).y * speedFactor;
        this->transform->position.z -= glm::column(transform, 2).z * speedFactor;
        orthoFOV += 0.5;
    }

    UpdateProjectionMatrix();

    previousViewMatrix = glm::mat4(viewMatrix);
    viewMatrix = glm::inverse(this->transform->GetModelMatrix());
}

Geometry::Ray CameraScene::GetRay(int x, int y) {
    Geometry::Ray ray;
    glm::mat4 localToWorld = this->transform->GetModelMatrix();

    int width = this->scene->windowWidth;
    int height = this->scene->windowHeight;
    float ndcX = ((((float)x / (float)width) * 2.0) - 1.0) * this->aspect;
    float ndcY = 1.0 - (((float)y / (float)height) * 2.0);
    float ndcZ = 1 / std::tan(this->fov/2.0);
    if(projectionType == ProjectionType::Perspective) {
        ray.origin = localToWorld *  glm::vec4(0, 0, 0, 1);
        ray.direction = glm::normalize(localToWorld * glm::vec4(ndcX, ndcY, ndcZ, 0));
    } else {
		glm::vec4 posCameraSpace = glm::vec4(orthoFOV * ndcX * this->aspect, orthoFOV *ndcY, 0.0, 1.0);
		ray.origin = glm::vec3(localToWorld * posCameraSpace);
		ray.direction = glm::vec3(localToWorld * glm::vec4(0.0, 0.0, 1.0, 0.0));
    }

    return ray;
}

Geometry::Ray CameraScene::GetRay(double x, double y) {
    Geometry::Ray ray;
    glm::mat4 localToWorld = this->transform->GetModelMatrix();

    int width = this->scene->windowWidth;
    int height = this->scene->windowHeight;

    double ndcX = (((x / (double)width) * 2.0) - 1.0) * this->aspect;
    double ndcY = 1.0 - ((y / (double)height) * 2.0);
    double ndcZ = 1 / std::tan(this->fov/2.0);
    if(projectionType == ProjectionType::Perspective) {
        ray.origin = localToWorld *  glm::vec4(0, 0, 0, 1);
        ray.direction = glm::normalize(localToWorld * glm::vec4(ndcX, ndcY, ndcZ, 0));

    } else {
		glm::vec4 posCameraSpace = glm::vec4(orthoFOV * ndcX * this->aspect, orthoFOV *ndcY, 0.0, 1.0);
		ray.origin = glm::vec3(localToWorld * posCameraSpace);
		ray.direction = glm::vec3(localToWorld * glm::vec4(0.0, 0.0, 1.0, 0.0));
	}
    return ray;
}

Geometry::Planes CameraScene::GetPlanes()
{
	glm::mat4 transformMatrix = glm::inverseTranspose(transform->GetModelMatrix());
	
	glm::mat4 projectionMatrix = GetProjectionMatrix();

	glm::vec4 nearPlane = glm::normalize(glm::row(projectionMatrix, 3) + glm::row(projectionMatrix, 2));
	glm::vec4 farPlane = glm::normalize(glm::row(projectionMatrix, 3) - glm::row(projectionMatrix, 2));

	glm::vec4 leftPlane = glm::normalize(glm::row(projectionMatrix, 3) + glm::row(projectionMatrix, 0));
	glm::vec4 rightPlane = glm::normalize(glm::row(projectionMatrix, 3) - glm::row(projectionMatrix, 0));

	glm::vec4 bottomPlane = glm::normalize(glm::row(projectionMatrix, 3) + glm::row(projectionMatrix, 1));
	glm::vec4 topPlane = glm::normalize(glm::row(projectionMatrix, 3) - glm::row(projectionMatrix, 1));

	Geometry::Planes planes = {
		transformMatrix * rightPlane,
		transformMatrix * leftPlane,
		transformMatrix * topPlane,
		transformMatrix * bottomPlane,
		transformMatrix * nearPlane,
		transformMatrix * farPlane
	};

	return planes;
}

}
}