#include "DrawImmediate.hpp"

#include "BaseObjects.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/MaterialComponent.hpp"
#include "Components/MeshFilterComponent.hpp"
#include "Components/BaseMeshes.hpp"

namespace KikooRenderer
{
	
	
namespace CoreEngine
{
DrawImmediate::DrawImmediate(Scene* s) {
    this->scene = s;
}

DrawImmediate::~DrawImmediate() {
    Destroy();
}

void DrawImmediate::Init() {
    wireframeCube =  GetWireFrameBox(scene, "DrawWireCube", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1));
    wireframeCube->Enable();
    wireframeCube->Start();


    wireframeSphere =  GetWireSphere(scene, "DrawWireCube", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1));
    wireframeSphere->Enable();
    wireframeSphere->Start(); 
    
    wireframeCapsule =  GetWireCapsule(scene, "DrawWireCube", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1));
    wireframeCapsule->Enable();
    wireframeCapsule->Start();    
    
    wireframeCone =  GetCone(scene, "DrawWireCube", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1));
    wireframeCone->Enable();
    wireframeCone->Start(); 
    
    wireframeCylinder =  GetWireCylinder(scene, "DrawWireCube", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1));
    wireframeCylinder->Enable();
    wireframeCylinder->Start();     
}

void DrawImmediate::Destroy() {
    delete wireframeCube;
}


void DrawImmediate::DrawWireBox(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4 color) {
    wireframeCube->transform->position = position;
    wireframeCube->transform->rotation = rotation;
    wireframeCube->transform->scale = scale;
    wireframeCube->GetComponent<MaterialComponent>()->albedo = color;

    wireframeCube->Render();
}   


void DrawImmediate::DrawWireSphere(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4(color)) {
    wireframeSphere->transform->position = position;
    wireframeSphere->transform->rotation = rotation;
    wireframeSphere->transform->scale = scale;
    wireframeSphere->GetComponent<MaterialComponent>()->albedo = color;

    wireframeSphere->Render();
}
void DrawImmediate::DrawWireCapsule(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, float height, float radius, glm::vec4(color)) {
    wireframeCapsule->transform->position = position;
    wireframeCapsule->transform->rotation = rotation;
    wireframeCapsule->transform->scale = scale;

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;
    GetCapsuleBuffers(&vertex, &normals, &uv, &colors, &triangles, height, radius);

    wireframeCapsule->GetComponent<MeshFilterComponent>()->LoadFromBuffers(vertex, normals, uv, colors, triangles, true);
    wireframeCapsule->GetComponent<MeshFilterComponent>()->RebuildBuffers();
  
   wireframeCapsule->GetComponent<MaterialComponent>()->albedo = color;

    wireframeCapsule->Render();
}        
void DrawImmediate::DrawWireCone(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4(color)) {
    wireframeCone->transform->position = position;
    wireframeCone->transform->rotation = rotation;
    wireframeCone->transform->scale = scale;
    wireframeCone->GetComponent<MaterialComponent>()->albedo = color;

    wireframeCone->Render();
}        
void DrawImmediate::DrawWireCylinder(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec4(color)) {
    wireframeCylinder->transform->position = position;
    wireframeCylinder->transform->rotation = rotation;
    wireframeCylinder->transform->scale = scale;


    wireframeCylinder->GetComponent<MaterialComponent>()->albedo = color;

    wireframeCylinder->Render();
}        

}
}