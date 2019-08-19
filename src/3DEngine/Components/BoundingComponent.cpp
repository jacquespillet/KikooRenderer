#include "BoundingComponent.hpp"
#include "../BaseObjects.hpp"
#include "MeshFilterComponent.hpp"
#include "TransformComponent.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
    BoundingBoxComponent::BoundingBoxComponent(Object3D* object) : Component("BoundingBox", object) {
    }
   
    void BoundingBoxComponent::OnStart() {    }
   
    void BoundingBoxComponent::OnEnable() {
        inited = false;
        min = glm::dvec3(9999, 9999, 9999);
        max = glm::dvec3(-9999, -9999, -9999);


        MeshFilterComponent* mesh = (MeshFilterComponent*) this->object3D->GetComponent("MeshFilter");
        transform = this->object3D->transform;
        
        for(int i=0; i<mesh->vertices.size(); i++) {
            if(mesh->vertices[i].position.x < min.x) min.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y < min.y) min.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z < min.z) min.z = mesh->vertices[i].position.z; 
            
            if(mesh->vertices[i].position.x > max.x) max.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y > max.y) max.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z > max.z) max.z = mesh->vertices[i].position.z;
        }
		
		positions.push_back(glm::dvec3(min.x, min.y, min.z));
		positions.push_back(glm::dvec3(min.x, max.y, min.z));
		positions.push_back(glm::dvec3(max.x, max.y, min.z));
		positions.push_back(glm::dvec3(max.x, min.y, min.z));
		positions.push_back(glm::dvec3(min.x, min.y, max.z));
		positions.push_back(glm::dvec3(min.x, max.y, max.z));
		positions.push_back(glm::dvec3(max.x, max.y, max.z));
		positions.push_back(glm::dvec3(max.x, min.y, max.z));

        
        
        glm::dvec3 position = (max + min) * 0.5;
        boxObject = GetWireFrameBox(object3D->scene, "BoundingBox", position, glm::dvec3(0), glm::dvec3(1), glm::dvec4(0, 1, 0, 1));
        inited = true;

    }

    void BoundingBoxComponent::Refresh() {
        min = glm::dvec3(9999, 9999, 9999);
        max = glm::dvec3(-9999, -9999, -9999);


        MeshFilterComponent* mesh = (MeshFilterComponent*) this->object3D->GetComponent("MeshFilter");
        transform = this->object3D->transform;
        
        for(int i=0; i<mesh->vertices.size(); i++) {
            if(mesh->vertices[i].position.x < min.x) min.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y < min.y) min.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z < min.z) min.z = mesh->vertices[i].position.z; 
            
            if(mesh->vertices[i].position.x > max.x) max.x = mesh->vertices[i].position.x; 
            if(mesh->vertices[i].position.y > max.y) max.y = mesh->vertices[i].position.y; 
            if(mesh->vertices[i].position.z > max.z) max.z = mesh->vertices[i].position.z;
        }
		
		positions.push_back(glm::dvec3(min.x, min.y, min.z));
		positions.push_back(glm::dvec3(min.x, max.y, min.z));
		positions.push_back(glm::dvec3(max.x, max.y, min.z));
		positions.push_back(glm::dvec3(max.x, min.y, min.z));
		positions.push_back(glm::dvec3(min.x, min.y, max.z));
		positions.push_back(glm::dvec3(min.x, max.y, max.z));
		positions.push_back(glm::dvec3(max.x, max.y, max.z));
		positions.push_back(glm::dvec3(max.x, min.y, max.z));

        
        
        glm::dvec3 position = (max + min) * 0.5;
        boxObject = GetWireFrameBox(object3D->scene, "BoundingBox", position, glm::dvec3(0), glm::dvec3(1), glm::dvec4(0, 1, 0, 1));
    }
    
    void BoundingBoxComponent::OnUpdate() {
        if(shouldRefresh) {
            Refresh();
            shouldRefresh = false;
        }
    }
    
    void BoundingBoxComponent::OnRender() {

    }
    
    void BoundingBoxComponent::OnDestroy() {

    }
    
    Object3D* BoundingBoxComponent::GetBoxObject() {
        return boxObject;
    }

    void BoundingBoxComponent::GetWorldBounds(glm::dvec3* _min, glm::dvec3* _max) {
		wmin = glm::dvec3(9999, 9999, 9999);
		wmax = glm::dvec3(-9999, -9999, -9999);
		glm::dmat4 modelMatrix = transform->GetWorldModelMatrix();

		for (int i = 0; i < positions.size(); i++) {
			glm::dvec4 wPos = modelMatrix * glm::dvec4(positions[i], 1);

			if (wPos.x < wmin.x) wmin.x = wPos.x;
			if (wPos.y < wmin.y) wmin.y = wPos.y;
			if (wPos.z < wmin.z) wmin.z = wPos.z;

			if (wPos.x > wmax.x) wmax.x = wPos.x;
			if (wPos.y > wmax.y) wmax.y = wPos.y;
			if (wPos.z > wmax.z) wmax.z = wPos.z;
		}

		
		(*_min) = wmin;
		(*_max) = wmax;
    }

    void BoundingBoxComponent::GetLocalBounds(glm::dvec3* _min, glm::dvec3* _max) {
        (*_min) =  glm::dvec4(this->min, 1.0);
        (*_max) =  glm::dvec4(this->max, 1.0);
    }
}
}