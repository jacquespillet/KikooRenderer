#include "BulletComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"
#include "3DEngine/Components/BoundingComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {

BulletPhysicsObjectInspector::BulletPhysicsObjectInspector(BulletPhysicsObjectComponent* _bulletPhysicsObjectComponent) : ComponentInspector("Bullet Physics Object", _bulletPhysicsObjectComponent)  {
    setLocale(QLocale("a")); //For . as , in float spin boxes

	this->bulletPhysicsObjectComponent = _bulletPhysicsObjectComponent;
	Object3D* object = bulletPhysicsObjectComponent->object3D;
	scene = object->scene;

	QVBoxLayout* mainLayout = new QVBoxLayout();

	
    CustomSlider* massSlider = new CustomSlider(0.0f, 3.0f, 0.01, "Mass", bulletPhysicsObjectComponent->mass);
    mainLayout->addLayout(massSlider);
    QObject::connect(massSlider, &CustomSlider::Modified, [this](double val) {
        // bulletPhysicsObjectComponent->mass = val;

		// //Remove the rigid body from the dynamics world
		
		// std::cout << "Size before removal " << bulletPhysicsObjectComponent->object3D->scene->GetSimulation().dynamicsWorld->getCollisionObjectArray().size() << std::endl;
		// bulletPhysicsObjectComponent->object3D->scene->GetSimulation().dynamicsWorld->removeRigidBody(bulletPhysicsObjectComponent->rigidBody);
		// std::cout << "Size after removal " << bulletPhysicsObjectComponent->object3D->scene->GetSimulation().dynamicsWorld->getCollisionObjectArray().size() << std::endl;
		// btVector3 inertia;
		// bulletPhysicsObjectComponent->rigidBody->getCollisionShape()->calculateLocalInertia( bulletPhysicsObjectComponent->mass, inertia );
		// bulletPhysicsObjectComponent->rigidBody->setMassProps(bulletPhysicsObjectComponent->mass, inertia);
		
		// // //Add the rigid body to the dynamics world
		// bulletPhysicsObjectComponent->object3D->scene->GetSimulation().dynamicsWorld->addRigidBody( bulletPhysicsObjectComponent->rigidBody );
		// std::cout << "Size after adding " << bulletPhysicsObjectComponent->object3D->scene->GetSimulation().dynamicsWorld->getCollisionObjectArray().size() << std::endl;


        // btScalar btmass(bulletPhysicsObjectComponent->mass);
        // bool isDynamic = (btmass != 0.f);
        // btVector3 localInertia(0, 0, 0);
        // if (isDynamic)
        //     bulletPhysicsObjectComponent->colShape->calculateLocalInertia(btmass, localInertia);
        
        // bulletPhysicsObjectComponent->rigidBody->setMassProps(btmass, localInertia);



		
        scene->triggerRefresh = true;
    });

    
	setLayout(mainLayout);
}

BulletPhysicsObjectComponent::BulletPhysicsObjectComponent(Object3D* object, double _mass, RIGID_BODY_SHAPE _shape) : Component("Bullet", object) {
	this->mass = _mass;
	this->bb = object->GetComponent<BoundingBoxComponent>();
	this->shape = _shape;

	glm::vec3 min, max;
	bb->GetLocalBounds(&min, &max);
	min *= object->transform->scale;
	max *= object->transform->scale;

	if(_shape == RIGID_BODY_SHAPE::BOX) {
		float sizeX = std::max(max.x - min.x, 0.00001f);
		float sizeY = std::max(max.y - min.y, 0.00001f);
		float sizeZ = std::max(max.z - min.z, 0.00001f);
		
		colShape =  new btBoxShape(btVector3(sizeX * 0.5,sizeY * 0.5,sizeZ * 0.5));
	} else if (_shape == RIGID_BODY_SHAPE::CONE) {
		colShape =  new btConeShapeZ(0.25, 1);
	} else if(_shape == RIGID_BODY_SHAPE::SPHERE) {
		colShape =  new btSphereShape(btScalar(object->transform->scale.x));
	}
	
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar mass(mass);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		colShape->calculateLocalInertia(mass, localInertia);

	startTransform.setOrigin(btVector3(object3D->transform->position.x, object3D->transform->position.y, object3D->transform->position.z));
	btQuaternion rotation;
	rotation.setEulerZYX (object3D->transform->rotation.z * DEGTORAD, object3D->transform->rotation.y * DEGTORAD, object3D->transform->rotation.x * DEGTORAD);
	startTransform.setRotation(rotation);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	rigidBody = new btRigidBody(rbInfo);

	if (_shape == RIGID_BODY_SHAPE::CONE || _shape == RIGID_BODY_SHAPE::SPHERE) {
		rigidBody->setFriction(1.f);
		rigidBody->setRollingFriction(.01);
		rigidBody->setSpinningFriction(0.01);
		rigidBody->setAnisotropicFriction(colShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	}


}

void BulletPhysicsObjectComponent::OnStart() {

}

void BulletPhysicsObjectComponent::OnEnable() {

}

void BulletPhysicsObjectComponent::OnUpdate() {

}

void BulletPhysicsObjectComponent::OnRender() {

}

void BulletPhysicsObjectComponent::OnDestroy() {

}

void BulletPhysicsObjectComponent::Recompute() {

}


ComponentInspector* BulletPhysicsObjectComponent::GetInspector() {
	bulletPhysicsObjectInspector = new BulletPhysicsObjectInspector(this);
	return bulletPhysicsObjectInspector;
}

void BulletPhysicsObjectComponent::FromJSON(QJsonObject json, Object3D* obj) {

	// glm::vec3 position;
	// QJsonObject positionJson = json["position"].toObject();
	// position.x = positionJson["X"].toDouble();
	// position.y = positionJson["Y"].toDouble();
	// position.z = positionJson["Z"].toDouble();
	
	// glm::vec3 rotation;
	// QJsonObject rotationJson = json["rotation"].toObject();
	// rotation.x = rotationJson["X"].toDouble();
	// rotation.y = rotationJson["Y"].toDouble();
	// rotation.z = rotationJson["Z"].toDouble();
	
	// glm::vec3 scale;
	// QJsonObject scaleJson = json["scale"].toObject();
	// scale.x = scaleJson["X"].toDouble();
	// scale.y = scaleJson["Y"].toDouble();
	// scale.z = scaleJson["Z"].toDouble();

	// BulletPhysicsObjectComponent* transform = new BulletPhysicsObjectComponent(obj);
	// transform->position = position;
	// transform->rotation = rotation;
	// transform->scale = scale;
	// obj->AddComponent(transform);
}

QJsonObject BulletPhysicsObjectComponent::ToJSON() {
	QJsonObject json;
	// json["type"] = "Transform";
	
	// QJsonObject positionJson;
	// positionJson["X"] = position.x;
	// positionJson["Y"] = position.y;
	// positionJson["Z"] = position.z;
	// json["position"] = positionJson;
	
	// QJsonObject scaleJson;
	// scaleJson["X"] = scale.x;
	// scaleJson["Y"] = scale.y;
	// scaleJson["Z"] = scale.z;
	// json["scale"] = scaleJson;
	
	// QJsonObject rotationJson;
	// rotationJson["X"] = rotation.x;
	// rotationJson["Y"] = rotation.y;
	// rotationJson["Z"] = rotation.z;
	// json["rotation"] = rotationJson;

	return json;
}


}
}