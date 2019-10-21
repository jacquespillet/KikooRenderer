#pragma once

#include "Util/Common.h"


#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btDeformableBodySolver.h>

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
enum WORLD_TYPE {RIGID, SOFT, DEFORMABLE};

class Simulation {
public:
    Simulation();

    void SetScene(CoreEngine::Scene* scene);
    void Init();
    void Simulate();
    void Destroy();

    void AddObject(CoreEngine::Object3D* object3D);
    
    btDiscreteDynamicsWorld* dynamicsWorld;

    WORLD_TYPE worldType;
protected:
    void GetSceneData();
    void SetSceneData();

    CoreEngine::Scene* scene;

    btDefaultCollisionConfiguration* collisionConfiguration;
    
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;

    btSequentialImpulseConstraintSolver* solver;
    btSoftBodySolver* softBodySolver;
    btDeformableBodySolver* deformableBodySolver;
    btDeformableMultiBodyConstraintSolver* deformableConstraintSolver;

    std::vector<CoreEngine::Object3D*> objects;
    std::vector<CoreEngine::Object3D*> softBodies;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;    
};
}
}
}