#pragma once

#include "Object3D.hpp"


namespace KikooRenderer {

namespace CoreEngine {
    Object3D* GetCube(Scene* scene);
    
    Object3D* GetQuad(Scene* scene);

    Object3D* GetCircle(Scene* scene);

    Object3D* GetSphere(Scene* scene);

    Object3D* GetGrid(Scene* scene);
    
    Object3D* GetAxes(Scene* scene);
    
    Object3D* GetCone(Scene* scene);
}
}