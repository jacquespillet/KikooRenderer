#include "Component.hpp"
#include "../Object3D.hpp"

namespace KikooRenderer {
namespace CoreEngine {

ComponentInspector::ComponentInspector(std::string name, Component* component) : QGroupBox(QString::fromStdString(name)) {
	this->component = component;
	Object3D* object = component->object3D;
	scene = object->scene;
}


Component::Component(std::string name, Object3D* object) {
    this->name = name;
    this->object3D = object;
}
}
}