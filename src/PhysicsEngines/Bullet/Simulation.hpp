#pragma once

#include "Util/Common.h"


#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedObjectArray.h>

namespace KikooRenderer {
namespace CoreEngine{
    class Scene;
    class Object3D;
}
namespace Physics {
namespace Bullet {

class Simulation {
public:
    Simulation();

    void SetScene(CoreEngine::Scene* scene);
    void Init();
    void Simulate();
    void Destroy();

    void AddObject(CoreEngine::Object3D* object3D);
protected:
    void GetSceneData();
    void SetSceneData();

    CoreEngine::Scene* scene;


    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    std::vector<CoreEngine::Object3D*> objects;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;    
};
}
}
}