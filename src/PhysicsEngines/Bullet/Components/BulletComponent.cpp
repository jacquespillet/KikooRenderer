#include "BulletComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"


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
        bulletPhysicsObjectComponent->mass = val;

		//Remove the rigid body from the dynamics world
		
		bulletPhysicsObjectComponent->object3D->scene->simulation.dynamicsWorld->removeRigidBody(bulletPhysicsObjectComponent->rigidBody);
		btVector3 inertia;
		bulletPhysicsObjectComponent->rigidBody->getCollisionShape()->calculateLocalInertia( mass, inertia );
		bulletPhysicsObjectComponent->rigidBody->setMassProps(mass, inertia);
		
		// //Add the rigid body to the dynamics world
		bulletPhysicsObjectComponent->object3D->scene->simulation.dynamicsWorld->addRigidBody( bulletPhysicsObjectComponent->rigidBody );


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

BulletPhysicsObjectComponent::BulletPhysicsObjectComponent(Object3D* object) : Component("Bullet", object) {

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