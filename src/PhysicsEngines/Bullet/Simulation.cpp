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
	collisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration); 
    
    GetSceneData();
}

void Simulation::AddObject(CoreEngine::Object3D* object3D) {
	CoreEngine::BulletPhysicsObjectComponent* physicsObjectComponent = object3D->GetComponent<CoreEngine::BulletPhysicsObjectComponent>();
	if(physicsObjectComponent != nullptr) {
		btCollisionShape* colShape = physicsObjectComponent->colShape;
		// btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		// btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(physicsObjectComponent->mass);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(object3D->transform->position.x, object3D->transform->position.y, object3D->transform->position.z));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		physicsObjectComponent->rigidBody = new btRigidBody(rbInfo);


		dynamicsWorld->addRigidBody(physicsObjectComponent->rigidBody);
		objects.push_back(object3D);	
	}
}

void Simulation::GetSceneData() {	  
}

void Simulation::Simulate() {
    dynamicsWorld->stepSimulation(1.f / 60.f, 10);

    //print positions of all objects
    for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
        btRigidBody* body = btRigidBody::upcast(obj);
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
    }

	scene->triggerRefresh = true;
}

void Simulation::SetSceneData() {
    //Loop through all physical objects in the scene
    //read position, rotation, scale, vertices
    //Set them back
    //Rebuild buffers if needed (Soft bodies)
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

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}

}
}
}