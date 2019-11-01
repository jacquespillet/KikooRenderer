#include "CameraScene.hpp"
#include "Components/TransformComponent.hpp"
#include "Scene.hpp"

#include "CameraControllers/CameraController.hpp"

namespace KikooRenderer {
namespace CoreEngine {

void InsertionSort(std::vector<Object3D*>& arr)  
{  
    Object3D* key;
    int i, j;  
    for (i = 1; i < arr.size(); i++) 
    {  
        key = arr[i];  
        j = i - 1;  
  
        while (j >= 0 && arr[j]->transform->camDistance < key->transform->camDistance) 
        {  
            arr[j + 1] = arr[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;  
    }  
} 

CameraScene::CameraScene(Scene* _scene, float _eyeDistance, float _fov, float _near, float _far, float _aspect) : Object3D("Camera", _scene) {
	transform = new TransformComponent(this);

    this->scene = _scene;
    this->eyeDistance = _eyeDistance;
    this->fov = _fov;
    this->nearClip = _near;
    this->farClip = _far;
    this->aspect = _aspect;

    UpdateProjectionMatrix();

    this->transform->position.x = 0;
    this->transform->position.y = 2;
    this->transform->position.z = -3;

	projectionType = ProjectionType::Perspective;
    viewMatrix = glm::inverse(this->transform->GetModelMatrix()); 

    orthoFOV = 10;

    cameraController = new CameraController(this);
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

void CameraScene::OnKeyReleaseEvent(QKeyEvent *e){
    cameraController->OnKeyReleaseEvent(e);
}

void CameraScene::OnUpdate(){
    transform->OnUpdate();
    cameraController->OnUpdate();

    if(transform->hasChanged ) {
        InsertionSort(scene->objects3D);  
        transform->hasChanged = false;
    }
}

void CameraScene::OnKeyPressEvent(QKeyEvent *e){
    
    cameraController->OnKeyPressEvent(e);
	if (e->key() == Qt::Key_5) {
		if (projectionType == ProjectionType::Perspective) projectionType = ProjectionType::Orthographic;
		else projectionType = ProjectionType::Perspective;
			
		UpdateProjectionMatrix();

	}
    previousViewMatrix = glm::mat4(viewMatrix);
    viewMatrix = glm::inverse(this->transform->GetModelMatrix());
}

void CameraScene::OnMousePressEvent(QMouseEvent *e) {
    cameraController->OnMousePressEvent(e);
}
void CameraScene::OnMouseReleaseEvent(QMouseEvent *e) {
    cameraController->OnMouseReleaseEvent(e);
}
void CameraScene::OnMouseMoveEvent(QMouseEvent *e) {
    cameraController->OnMouseMoveEvent(e);

    previousViewMatrix = glm::mat4(viewMatrix);
    viewMatrix = glm::inverse(this->transform->GetModelMatrix());
}
void CameraScene::OnWheelEvent(QWheelEvent *e) {
    cameraController->OnWheelEvent(e);

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

float CameraScene::GetScreenScale(glm::vec3 objPos)
{
    glm::vec3 campos = glm::vec3(transform->GetModelMatrix() * glm::vec4(0, 0, 0, 1)); 
    float dist = glm::length(campos - objPos); 
    return dist * tan(fov / 2 * DEGTORAD); 
}


//Accessors
float CameraScene::GetEyeDistance() {
    return eyeDistance;
}

void  CameraScene::SetEyeDistance(float value) {
    this->eyeDistance = value;
}

float CameraScene::GetFov() {
    return fov;
}

void  CameraScene::SetFov(float value) {
    this->fov = value;
}

float CameraScene::GetNearClip() {
    return nearClip;
}

void  CameraScene::SetNearClip(float value) {
    this->nearClip = value;
}

float CameraScene::GetFarClip() {
    return farClip;
}

void  CameraScene::SetFarClip(float value) {
    this->farClip = value;
}

float CameraScene::GetAspect() {
    return aspect;
}

void  CameraScene::SetAspect(float value) {
    this->aspect = value;
}


void  CameraScene::SetProjectionMatrix(glm::mat4 value) {
    this->projectionMatrix = value;
}

void  CameraScene::SetViewMatrix(glm::mat4 value) {
    this->viewMatrix = value;
}

glm::mat4 CameraScene::GetPreviousViewMatrix() {
    return previousViewMatrix;
}

void  CameraScene::SetPreviousViewMatrix(glm::mat4 value) {
    this->previousViewMatrix = value;
}

CameraController* CameraScene::GetCameraController() {
    return cameraController;
}

void  CameraScene::SetCameraController(CameraController* value) {
    this->cameraController = value;
}

float CameraScene::GetOrthoFOV() {
    return orthoFOV;
}

void  CameraScene::SetOrthoFOV(float value) {
    this->orthoFOV = value;
}

float CameraScene::GetSpeedFactor() {
    return speedFactor;
}

void  CameraScene::SetSpeedFactor(float value) {
    this->speedFactor = value;
}


}
}