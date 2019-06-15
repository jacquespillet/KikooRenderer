#pragma once

#include "Object3D.hpp"


namespace KikooRenderer {

namespace CoreEngine {
    Object3D* GetCube(Scene* scene, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color);
    
    Object3D* GetQuad(Scene* scene, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color);

    Object3D* GetCircle(Scene* scene, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color);

    Object3D* GetSphere(Scene* scene, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color);

    Object3D* GetGrid(Scene* scene);
    
    Object3D* GetAxes(Scene* scene);
    
    Object3D* GetCone(Scene* scene, glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale, glm::dvec4 _color);

    Object3D* GetLine(Scene* scene, glm::dvec3 position1, glm::dvec3 position2, glm::dvec4 color);

    Object3D* GetTranslateWidget(Scene* scene,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale);

    Object3D* GetScaleWidget(Scene* scene,glm::dvec3 _position, glm::dvec3 _rotation, glm::dvec3 _scale);
}
}