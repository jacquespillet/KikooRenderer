#include "BoundingComponent.hpp"
#include "../BaseObjects.hpp"
#include "MeshFilterComponent.hpp"
#include "TransformComponent.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {


BoundingBoxInspector::BoundingBoxInspector(BoundingBoxComponent* boundingBoxComponent) : ComponentInspector("Bounding Box", boundingBoxComponent) {
	this->boundingBoxComponent = boundingBoxComponent;
	Object3D* object = boundingBoxComponent->object3D;
	scene = object->scene;
	mainLayout = new QVBoxLayout();

    QCheckBox* showBoxCheckbox = new QCheckBox("Show");
    mainLayout->addWidget(showBoxCheckbox);
    connect(showBoxCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
        this->boundingBoxComponent->isShown = !this->boundingBoxComponent->isShown;
        scene->triggerRefresh = true;
    });

	setLayout(mainLayout);
}

void BoundingBoxInspector::Refresh() {}

BoundingBoxComponent::BoundingBoxComponent(Object3D* object) : Component("BoundingBox", object) {
}

void BoundingBoxComponent::OnStart() {    }

void BoundingBoxComponent::OnEnable() {
    inited = false;
    min = glm::vec3(9999, 9999, 9999);
    max = glm::vec3(-9999, -9999, -9999);


    MeshFilterComponent* mesh = this->object3D->GetComponent<MeshFilterComponent>();
    transform = this->object3D->transform;
    
    for(int i=0; i<mesh->vertices.size(); i++) {
        if(mesh->vertices[i].position.x < min.x) min.x = mesh->vertices[i].position.x; 
        if(mesh->vertices[i].position.y < min.y) min.y = mesh->vertices[i].position.y; 
        if(mesh->vertices[i].position.z < min.z) min.z = mesh->vertices[i].position.z; 
        
        if(mesh->vertices[i].position.x > max.x) max.x = mesh->vertices[i].position.x; 
        if(mesh->vertices[i].position.y > max.y) max.y = mesh->vertices[i].position.y; 
        if(mesh->vertices[i].position.z > max.z) max.z = mesh->vertices[i].position.z;
    }
    
    positions.push_back(glm::vec3(min.x, min.y, min.z));
    positions.push_back(glm::vec3(min.x, max.y, min.z));
    positions.push_back(glm::vec3(max.x, max.y, min.z));
    positions.push_back(glm::vec3(max.x, min.y, min.z));
    positions.push_back(glm::vec3(min.x, min.y, max.z));
    positions.push_back(glm::vec3(min.x, max.y, max.z));
    positions.push_back(glm::vec3(max.x, max.y, max.z));
    positions.push_back(glm::vec3(max.x, min.y, max.z));

    
    
    glm::vec3 position = (max + min) * 0.5;
    glm::vec3 size = max - min;
    boxObject = GetWireFrameBox(object3D->scene, "BoundingBox", position, glm::vec3(0), size, glm::vec4(0, 1, 0, 1));
    boxObject->Start();
    boxObject->Enable();
    inited = true;
}

void BoundingBoxComponent::Refresh() {
    min = glm::vec3(9999, 9999, 9999);
    max = glm::vec3(-9999, -9999, -9999);


    MeshFilterComponent* mesh = this->object3D->GetComponent<MeshFilterComponent>();
    transform = this->object3D->transform;
    
    for(int i=0; i<mesh->vertices.size(); i++) {
        if(mesh->vertices[i].position.x < min.x) min.x = mesh->vertices[i].position.x; 
        if(mesh->vertices[i].position.y < min.y) min.y = mesh->vertices[i].position.y; 
        if(mesh->vertices[i].position.z < min.z) min.z = mesh->vertices[i].position.z; 
        
        if(mesh->vertices[i].position.x > max.x) max.x = mesh->vertices[i].position.x; 
        if(mesh->vertices[i].position.y > max.y) max.y = mesh->vertices[i].position.y; 
        if(mesh->vertices[i].position.z > max.z) max.z = mesh->vertices[i].position.z;
    }
    
    positions.push_back(glm::vec3(min.x, min.y, min.z));
    positions.push_back(glm::vec3(min.x, max.y, min.z));
    positions.push_back(glm::vec3(max.x, max.y, min.z));
    positions.push_back(glm::vec3(max.x, min.y, min.z));
    positions.push_back(glm::vec3(min.x, min.y, max.z));
    positions.push_back(glm::vec3(min.x, max.y, max.z));
    positions.push_back(glm::vec3(max.x, max.y, max.z));
    positions.push_back(glm::vec3(max.x, min.y, max.z));

    
    
    glm::vec3 position = (max + min) * 0.5;
    glm::vec3 size = max - min;
    boxObject = GetWireFrameBox(object3D->scene, "BoundingBox", position, glm::vec3(0), glm::vec3(size), glm::vec4(0, 1, 0, 1));
    boxObject->Start();
    boxObject->Enable();
}

void BoundingBoxComponent::OnUpdate() {
    if(shouldRefresh) {
        Refresh();
        shouldRefresh = false;
    }
}


ComponentInspector* BoundingBoxComponent::GetInspector() {
    boundingBoxInspector = new BoundingBoxInspector(this);
    return boundingBoxInspector;
}    

void BoundingBoxComponent::OnRender() {
    if(isShown) {
        boxObject->transform->position = object3D->transform->position;
        boxObject->transform->rotation = object3D->transform->rotation;
        // boxObject->transform->scale = object3D->transform->scale;
        // std::cout << glm::to_string(boxObject->transform->scale) << std::endl;
        boxObject->Render();
    }
}

void BoundingBoxComponent::OnDestroy() {

}

Object3D* BoundingBoxComponent::GetBoxObject() {
    return boxObject;
}

void BoundingBoxComponent::GetWorldBounds(glm::vec3* _min, glm::vec3* _max) {
    wmin = glm::vec3(9999, 9999, 9999);
    wmax = glm::vec3(-9999, -9999, -9999);
    glm::mat4 modelMatrix = transform->GetWorldModelMatrix();

    for (int i = 0; i < positions.size(); i++) {
        glm::vec4 wPos = modelMatrix * glm::vec4(positions[i], 1);

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

void BoundingBoxComponent::GetNDCBounds(glm::vec2& min, glm::vec2& max, glm::mat4 mvpMat) {
    min = glm::vec2(999999999, 9999999999);
    max = glm::vec2(-99999999999, -9999999999);
    for (int i = 0; i < positions.size(); i++) {
        glm::vec4 sPos = mvpMat * glm::vec4(positions[i], 1);
        sPos /= sPos.w;

        if (sPos.x < min.x) min.x = sPos.x;
        if (sPos.y < min.y) min.y = sPos.y;

        if (sPos.x > max.x) max.x = sPos.x;
        if (sPos.y > max.y) max.y = sPos.y;
    }
}

void BoundingBoxComponent::GetLocalBounds(glm::vec3* _min, glm::vec3* _max) {
    (*_min) =  glm::vec4(this->min, 1.0);
    (*_max) =  glm::vec4(this->max, 1.0);
}

void BoundingBoxComponent::FromJSON(QJsonObject json, Object3D* obj) {
    BoundingBoxComponent* bb = new BoundingBoxComponent(obj);
    obj->AddComponent(bb);
} 

QJsonObject BoundingBoxComponent::ToJSON() {
    QJsonObject json;
    json["type"] = "BoundingBox";

    QJsonObject minJson;
    minJson["x"] = min.x;
    minJson["y"] = min.y;
    minJson["z"] = min.z;
    json["min"] = minJson;

    QJsonObject maxJson;
    maxJson["x"] = max.x;
    maxJson["y"] = max.y;
    maxJson["z"] = max.z;
    json["max"] = maxJson;



    return json;
}    

}
}