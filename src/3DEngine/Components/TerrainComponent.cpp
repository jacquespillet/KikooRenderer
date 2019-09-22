#include "TerrainComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"
#include "MeshFilterComponent.hpp"
#include "Util/NoiseUtil.hpp"

namespace KikooRenderer {
namespace CoreEngine {
float TerrainComponent::GetXDerivative(int x, int z, int vInx) {
    float left, right, scale;
    if (x > 0) {
        left = mesh->vertices[vInx - 1].position.y;
        if (x < subdivX) {
            right = mesh->vertices[vInx + 1].position.y;
            scale = 0.5f * subdivX;
        }
        else {
            right = mesh->vertices[vInx].position.y;
            scale = subdivX;
        }
    }
    else {
        left = mesh->vertices[vInx].position.y;
        right = mesh->vertices[vInx + 1].position.y;
        scale = subdivX;
    }
    return (right - left) * scale;
}

float TerrainComponent::GetZDerivative (int x, int z, int vInx) {
    float back, forward, scale;
    if (z > 0) {
        back = mesh->vertices[vInx - subdivX].position.y;
        if (z < subdivY) {
            forward = mesh->vertices[vInx + subdivX].position.y;
            scale = 0.5f * subdivY; 
        }
        else {
            forward = mesh->vertices[vInx].position.y;
            scale = subdivY;
        }
    }
    else {
        back = mesh->vertices[vInx].position.y;
        forward = mesh->vertices[vInx + subdivX].position.y;
        scale = subdivY;
    }
    return (forward - back) * scale;
}

TerrainComponent::TerrainComponent(Object3D* object, float width,float height,int subdivX, int subdivY) : Component("Terrain", object) {
    mesh = (MeshFilterComponent*) object->GetComponent("MeshFilter");
    this->width = width;
    this->height = height;
    this->subdivX = subdivX;
    this->subdivY = subdivY;

    for(int i=0; i<mesh->vertices.size(); i++) {
        float xPos = mesh->vertices[i].position.x / width;
        float yPos = mesh->vertices[i].position.z / height;
        KikooRenderer::Util::NoiseSample sample = KikooRenderer::Util::GetFracNoise2D(xPos,yPos, 2, 6, 2.0, 0.5);
        float color = sample.value;
        color = color * 0.5 + 0.5;
        color *= 4.0;
        mesh->vertices[i].position.y = color;
        mesh->vertices[i].normal = glm::normalize(glm::vec3(-sample.derivative.x, 1.0f, -sample.derivative.y));        
    }

    mesh->RebuildBuffers();
    object3D->scene->triggerRefresh = true;     
}
void TerrainComponent::OnStart(){}
void TerrainComponent::OnEnable(){}
void TerrainComponent::OnUpdate(){
    for(int i=0; i<mesh->vertices.size(); i++) {
        float xPos = mesh->vertices[i].position.x / width;
        float yPos = mesh->vertices[i].position.z / height;
        KikooRenderer::Util::NoiseSample sample = KikooRenderer::Util::GetFracNoise3D(xPos,yPos, object3D->scene->elapsedTime, 2, 6, 2.0, 0.5);
        float color = sample.value;
        color = color * 0.5 + 0.5;
        color *= 4.0;
        mesh->vertices[i].position.y = color;
        mesh->vertices[i].normal = glm::normalize(glm::vec3(-sample.derivative.x, 1.0f, -sample.derivative.y));        
    }
    
    mesh->RebuildBuffers();
    object3D->scene->triggerRefresh = true;    
}
void TerrainComponent::OnRender(){} 
void TerrainComponent::OnDestroy(){} 
void TerrainComponent::Recompute() {}

QJsonObject TerrainComponent::ToJSON() {
	QJsonObject json;
	return json;
}
}
}