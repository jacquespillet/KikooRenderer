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
    // this->scene = _scene;
}

void Simulation::SetWorld(WORLD_TYPE _worldType) {
	// Destroy(false);
	// worldType = _worldType;
	// Init();
	// for(int i=0; i<scene->objects3D.size(); i++) {
	// 	CoreEngine::BulletPhysicsObjectComponent* physicsComponent = scene->objects3D[i]->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	// 	// physicsComponent->Init(); // ?????????????????
	// 	if(physicsComponent != nullptr) {
	// 		if(worldType == WORLD_TYPE::RIGID && (physicsComponent->bodyType == CoreEngine::BODY_TYPE::SOFT || physicsComponent->bodyType == CoreEngine::BODY_TYPE::DEFORMABLE)) {
	// 			//DONT ADD IT IF NOT COMPATIBLE WITH CURRENT WORLD
	// 		} else {
	// 			AddObject(scene->objects3D[i]);
	// 		}
	// 	}
	// }
}

void Simulation::Init() {
	if(worldType == WORLD_TYPE::RIGID) {
		// default setup for memory, collision setup. Advanced users can create their own configuration.
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
		
	} else if(worldType == WORLD_TYPE::SOFT) {
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
		btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*) dynamicsWorld;

		world->getDispatchInfo().m_enableSPU = true;
		world->setGravity(btVector3(0, -10, 0));
		world->getWorldInfo().m_sparsesdf.Initialize();
		world->getWorldInfo().air_density = (btScalar)1.2;
		world->getWorldInfo().water_density = 0;
		world->getWorldInfo().water_offset = 0;
		world->getWorldInfo().water_normal = btVector3(0, 0, 0);
	
	} else if(worldType == WORLD_TYPE::DEFORMABLE) {
		collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();

		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		overlappingPairCache= new btDbvtBroadphase();
    	deformableBodySolver = new btDeformableBodySolver();

		deformableConstraintSolver = new btDeformableMultiBodyConstraintSolver();
    	deformableConstraintSolver->setDeformableSolver(deformableBodySolver);

		dynamicsWorld = new btDeformableMultiBodyDynamicsWorld(dispatcher, overlappingPairCache, deformableConstraintSolver, collisionConfiguration, deformableBodySolver);
		dynamicsWorld->setGravity(btVector3(0, -10, 0));
		
		btDeformableMultiBodyDynamicsWorld* world = (btDeformableMultiBodyDynamicsWorld*) dynamicsWorld;

		world->setImplicit(true);
    	world->setLineSearch(false);
		btVector3 gravity = btVector3(0, -10, 0);
		world->setGravity(gravity);		
	}
    GetSceneData();
}

void Simulation::AddObject(CoreEngine::Object3D* object3D) {
	// if(worldType == WORLD_TYPE::RIGID) {
	// 	CoreEngine::BulletPhysicsObjectComponent* physicsObjectComponent = object3D->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	// 	if(physicsObjectComponent != nullptr) {
	// 		collisionShapes.push_back(physicsObjectComponent->colShape);
	// 		dynamicsWorld->addRigidBody(physicsObjectComponent->rigidBody);
	// 		objects.push_back(object3D);
	// 	}
	// }else if(worldType == WORLD_TYPE::SOFT) {
	// 	CoreEngine::BulletPhysicsObjectComponent* physicsObjectComponent = object3D->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	// 	if(physicsObjectComponent != nullptr) {
	// 		collisionShapes.push_back(physicsObjectComponent->colShape);
	// 		btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*) dynamicsWorld; 
	// 		if(physicsObjectComponent->bodyType == CoreEngine::BODY_TYPE::RIGID) {
	// 			world->addRigidBody(physicsObjectComponent->rigidBody);
	// 		} else {
	// 			world->addSoftBody(physicsObjectComponent->softBody);
	// 			softBodies.push_back(object3D);
	// 		}
	// 		objects.push_back(object3D);
	// 	}
	// }else if(worldType == WORLD_TYPE::DEFORMABLE) {
	// 	CoreEngine::BulletPhysicsObjectComponent* physicsObjectComponent = object3D->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	// 	if(physicsObjectComponent != nullptr) {
	// 		collisionShapes.push_back(physicsObjectComponent->colShape);
	// 		btDeformableMultiBodyDynamicsWorld* world = (btDeformableMultiBodyDynamicsWorld*) dynamicsWorld; 
	// 		if(physicsObjectComponent->bodyType == CoreEngine::BODY_TYPE::RIGID) {
	// 			world->addRigidBody(physicsObjectComponent->rigidBody);
	// 		} else {
	// 			world->addSoftBody(physicsObjectComponent->softBody);
	// 			softBodies.push_back(object3D);
	// 		}
	// 		objects.push_back(object3D);
	// 	}
	// }

	// for(int i=0; i<objects.size(); i++) {
	// 	Transform t;
	// 	t.position = objects[i]->transform->position;
	// 	t.rotation = objects[i]->transform->rotation;
	// 	initialTransforms[objects[i]] = t;
	// }	
}

void Simulation::RemoveObject(CoreEngine::Object3D* object3D) {
	// CoreEngine::BulletPhysicsObjectComponent* physicsObjectComponent = object3D->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	// if(physicsObjectComponent != nullptr) {
	// 	dynamicsWorld->removeRigidBody(physicsObjectComponent->rigidBody);

	// 	for(int i=0; i<objects.size(); i++) {
	// 		if(objects[i] == object3D) {
	// 			objects.erase(objects.begin() + i);
	// 			break;
	// 		}
	// 	}

	// 	for(int i=0; i<softBodies.size(); i++) {
	// 		if(softBodies[i] == object3D) {
	// 			softBodies.erase(softBodies.begin() + i);
	// 			break;
	// 		}
	// 	}

	// 	initialTransforms.erase(object3D);

	// 	collisionShapes.remove (physicsObjectComponent->colShape);
	// }
}

void Simulation::GetSceneData() {
}

void Simulation::Simulate() {
    // dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    // //print positions of all objects
    // for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    // {
    //     btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
    //     btRigidBody* body = btRigidBody::upcast(obj);
	// 	if(body) {
	// 		btTransform trans;
	// 		if (body && body->getMotionState())
	// 		{
	// 			body->getMotionState()->getWorldTransform(trans);
	// 		}
	// 		else
	// 		{
	// 			trans = obj->getWorldTransform();
	// 		}

	// 		objects[j]->transform->position.x = trans.getOrigin().getX();
	// 		objects[j]->transform->position.y = trans.getOrigin().getY();
	// 		objects[j]->transform->position.z = trans.getOrigin().getZ();


	// 		btQuaternion rotation = trans.getRotation();
	// 		btScalar rotX, rotY, rotZ;
	// 		rotation.getEulerZYX (rotZ, rotY, rotX);
	// 		objects[j]->transform->rotation.z = rotZ * RADTODEG;
	// 		objects[j]->transform->rotation.y = rotY* RADTODEG;
	// 		objects[j]->transform->rotation.x = rotX* RADTODEG;
	// 	}
    // }

	// if(worldType == WORLD_TYPE::SOFT) {
	// 	btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*) dynamicsWorld;
	// 	for(int i=0; i<world->getSoftBodyArray().size(); i++) {
	// 		btSoftBody* body = world->getSoftBodyArray()[i];
	// 		CoreEngine::Object3D* object = softBodies[i];
	// 		CoreEngine::MeshFilterComponent* mesh = object->GetComponent<CoreEngine::MeshFilterComponent>();
	// 		for(int j=0; j<mesh->vertices.size(); j++) {
	// 			btSoftBody::Node n = body->m_nodes[j];
				
	// 			mesh->vertices[j].position.x = n.m_x.getX();
	// 			mesh->vertices[j].position.y = n.m_x.getY();
	// 			mesh->vertices[j].position.z = n.m_x.getZ();

	// 			mesh->vertices[j].normal.x = n.m_n.getX();
	// 			mesh->vertices[j].normal.z = n.m_n.getY();
	// 			mesh->vertices[j].normal.y = n.m_n.getZ();
	// 		}
	// 		mesh->RebuildBuffers();
	// 	}
	// }

	// if(worldType == WORLD_TYPE::DEFORMABLE) {
	// 	btDeformableMultiBodyDynamicsWorld* world = (btDeformableMultiBodyDynamicsWorld*) dynamicsWorld; 
	// 	for(int i=0; i<world->getSoftBodyArray().size(); i++) {
	// 		btSoftBody* body = world->getSoftBodyArray()[i];
	// 		CoreEngine::Object3D* object = softBodies[i];
	// 		CoreEngine::MeshFilterComponent* mesh = object->GetComponent<CoreEngine::MeshFilterComponent>();
			
	// 		for(int j=0; j<mesh->vertices.size(); j++) {
	// 			btSoftBody::Node n = body->m_nodes[j];
				
	// 			mesh->vertices[j].position.x = n.m_x.getX();
	// 			mesh->vertices[j].position.y = n.m_x.getY();
	// 			mesh->vertices[j].position.z = n.m_x.getZ();

	// 			mesh->vertices[j].normal.x = n.m_n.getX();
	// 			mesh->vertices[j].normal.z = n.m_n.getY();
	// 			mesh->vertices[j].normal.y = n.m_n.getZ();
	// 		}
		
			
	// 		mesh->RebuildBuffers();
	// 	}
	// }


	// scene->triggerRefresh = true;
}

void Simulation::SetSceneData() {
}

void Simulation::Stop() {
	// for(int i=0; i<objects.size(); i++) {
	// 	Transform t = initialTransforms[objects[i]];
	// 	objects[i]->transform->position = t.position;
	// 	objects[i]->transform->rotation = t.rotation;
	// 	objects[i]->transform->hasChanged = true;
	// }
	// scene->triggerRefresh = true;
}

void Simulation::Destroy(bool destroyObjects) {
	objects.clear();
    softBodies.clear();
	
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			if(destroyObjects) delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		if(destroyObjects) delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		if(destroyObjects) delete shape;
	}

	//delete dynamics world
	if(dynamicsWorld != nullptr) delete dynamicsWorld;

	//delete solver
	if(solver != nullptr) delete solver;

	//delete broadphase
	if(overlappingPairCache != nullptr) delete overlappingPairCache;

	//delete dispatcher
	if(dispatcher != nullptr) delete dispatcher;

	if(collisionConfiguration != nullptr) delete collisionConfiguration;

	if(softBodySolver != nullptr) delete softBodySolver;

	if(deformableBodySolver != nullptr) delete deformableBodySolver;
	
	if(deformableConstraintSolver != nullptr) delete deformableConstraintSolver;

	collisionShapes.clear();
}

}
}
}