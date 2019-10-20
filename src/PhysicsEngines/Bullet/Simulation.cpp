#include "Simulation.hpp"
#include "3DEngine/Scene.hpp"
#include "3DEngine/Object3D.hpp"

#include "Components/BulletComponent.hpp"

namespace KikooRenderer {
namespace Physics {
namespace Bullet {

Simulation::Simulation() {
    
}


void Simulation::SetScene(CoreEngine::Scene* _scene) {
    this->scene = _scene;
}

void Simulation::Init() {
	//collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	// collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	
	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	// overlappingPairCache = new btDbvtBroadphase();
	btVector3 worldAabbMin(-1000, -1000, -1000);
	btVector3 worldAabbMax(1000, 1000, 1000);
	overlappingPairCache = new btAxisSweep3(worldAabbMin, worldAabbMax);

	//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	softBodySolver = new btDefaultSoftBodySolver();

	// dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration); 
	dynamicsWorld = new btSoftRigidDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration, softBodySolver); 

	// m_dynamicsWorld->setInternalTickCallback(pickingPreTickCallback, this, true);
	dynamicsWorld->getDispatchInfo().m_enableSPU = true;
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	dynamicsWorld->getWorldInfo().m_sparsesdf.Initialize();
	dynamicsWorld->getWorldInfo().air_density = (btScalar)1.2;
	dynamicsWorld->getWorldInfo().water_density = 0;
	dynamicsWorld->getWorldInfo().water_offset = 0;
	dynamicsWorld->getWorldInfo().water_normal = btVector3(0, 0, 0);


    GetSceneData();
}

void Simulation::AddObject(CoreEngine::Object3D* object3D) {
	CoreEngine::BulletPhysicsObjectComponent* physicsObjectComponent = object3D->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	if(physicsObjectComponent != nullptr) {
		collisionShapes.push_back(physicsObjectComponent->colShape);
		if(physicsObjectComponent->bodyType == CoreEngine::BODY_TYPE::RIGID) {
			dynamicsWorld->addRigidBody(physicsObjectComponent->rigidBody);
		} else {
			dynamicsWorld->addSoftBody(physicsObjectComponent->softBody);
			softBodies.push_back(object3D);
		}
		objects.push_back(object3D);
	}
}

void Simulation::GetSceneData() {
}

void Simulation::Simulate() {
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    //print positions of all objects
	// std::cout << dynamicsWorld->getNumCollisionObjects() << std::endl;
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
		if(body) {
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}

			objects[j]->transform->position.x = trans.getOrigin().getX();
			objects[j]->transform->position.y = trans.getOrigin().getY();
			objects[j]->transform->position.z = trans.getOrigin().getZ();


			btQuaternion rotation = trans.getRotation();
			btScalar rotX, rotY, rotZ;
			rotation.getEulerZYX (rotZ, rotY, rotX);
			objects[j]->transform->rotation.z = rotZ * RADTODEG;
			objects[j]->transform->rotation.y = rotY* RADTODEG;
			objects[j]->transform->rotation.x = rotX* RADTODEG;
		}
    }

	// std::cout << dynamicsWorld->getSoftBodyArray().size() << std::endl;
	for(int i=0; i<dynamicsWorld->getSoftBodyArray().size(); i++) {
		btSoftBody* body = dynamicsWorld->getSoftBodyArray()[i];
		CoreEngine::Object3D* object = softBodies[i];
		// std::cout << object->transform->position.y << std::endl;
		CoreEngine::MeshFilterComponent* mesh = object->GetComponent<CoreEngine::MeshFilterComponent>();
		for(int j=0; j<mesh->vertices.size(); j++) {
			btSoftBody::Node n = body->m_nodes[j];
			
			mesh->vertices[j].position.x = n.m_x.getX();
			mesh->vertices[j].position.y = n.m_x.getY();
			mesh->vertices[j].position.z = n.m_x.getZ();

			mesh->vertices[j].normal.x = n.m_n.getX();
			mesh->vertices[j].normal.z = n.m_n.getY();
			mesh->vertices[j].normal.y = n.m_n.getZ();
		}
		mesh->RebuildBuffers();
	}

	scene->triggerRefresh = true;
}

void Simulation::SetSceneData() {
}


void Simulation::Destroy() {
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	delete softBodySolver;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}

}
}
}