#include "Component.hpp"

namespace KikooRenderer {
namespace CoreEngine {

Component::Component(std::string name, Object3D* object) {
    this->name = name;
    this->object3D = object;
}
}
}