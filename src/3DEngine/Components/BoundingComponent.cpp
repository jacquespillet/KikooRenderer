#include "BoundingComponent.hpp"
#include "MeshFilterComponent.hpp"
#include "../BaseObjects.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
    BoundingBoxComponent::BoundingBoxComponent(Object3D* object) : Component("BoundingBox", object) {
        inited = false;
        min = glm::dvec3(9999, 9999, 9999);
        max = glm::dvec3(-9999, -9999, -9999);
    }
   
    void BoundingBoxComponent::OnStart() {    }
   
    void BoundingBoxComponent::OnEnable() {
        MeshFilterComponent* mesh = (MeshFilterComponent*) this->object3D->GetComponent("MeshFilter");
        for(int i=0; i<mesh->vertices.size(); i++) {
            if(mesh->vertices[i].position.x < min.x) min.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y < min.y) min.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z < min.z) min.z = mesh->vertices[i].position.z; 
            
            if(mesh->vertices[i].position.x > max.x) max.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y > max.y) max.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z > max.z) max.z = mesh->vertices[i].position.z;
        }
        
        glm::dvec3 position = (max + min) / 2.0;
        glm::dvec3 scale = max - min;
        boxObject = GetWireFrameBox(object3D->scene, position, glm::dvec3(0), scale, glm::dvec4(0, 1, 0, 1));
        inited = true;
    }
    
    void BoundingBoxComponent::OnUpdate() {
    }
    
    void BoundingBoxComponent::OnRender() {

    }
    
    void BoundingBoxComponent::OnDestroy() {

    }
    
    Object3D* BoundingBoxComponent::GetBoxObject() {
        std::cout << "getting box" << std::endl;
        return boxObject;
    }
}
}