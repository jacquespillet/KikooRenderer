#include "Component.hpp"
#include "../Object3D.hpp"
#include "BoundingComponent.hpp"
#include "MaterialComponent.hpp"
#include "FluidComponent.hpp"
#include "LightComponent.hpp"
#include "MeshFilterComponent.hpp"
#include "TerrainComponent.hpp"
#include "TransformComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {

ComponentInspector::ComponentInspector(std::string name, Component* component) : QGroupBox(QString::fromStdString(name)) {
	std::cout << "0" << std::endl;
	this->component = component;
	std::cout << "1" << std::endl;
	Object3D* object = component->object3D;
	std::cout << "2" << std::endl;
	scene = object->scene;
	std::cout << "3" << std::endl;
}


Component::Component(std::string name, Object3D* object) {
    this->name = name;
    this->object3D = object;
}

void Component::FromJSON(QJsonObject json, Object3D* obj) {
	QString type = json["type"].toString(); 
	if(type=="BoundingBox") {   
		BoundingBoxComponent::FromJSON(json, obj);
	} else if(type=="Light") {
		LightComponent::FromJSON(json, obj); 
	} else if(type=="Material") {
		MaterialComponent::FromJSON(json, obj);
	}else if(type=="MeshFilter") {
		MeshFilterComponent::FromJSON(json, obj);
	} else if(type=="Terrain") {
		TerrainComponent::FromJSON(json, obj);
	} else if(type=="Fluid") {
		FluidComponent::FromJSON(json, obj);
	}
}

}
}