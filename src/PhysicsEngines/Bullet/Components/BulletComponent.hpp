#pragma once 
#include "Util/Common.h"
#include "3DEngine/Components/Component.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class Scene;
class MeshFilterComponent;
class TransformComponent;
class BulletPhysicsObjectComponent;

class BulletPhysicsObjectInspector : public ComponentInspector {
	Q_OBJECT
public:
	BulletPhysicsObjectInspector(BulletPhysicsObjectComponent* BulletPhysicsObjectComponent);
	BulletPhysicsObjectComponent* bulletPhysicsObjectComponent;
	Scene* scene;

	// QDoubleSpinBox* xPositionSpinBox;
	// QDoubleSpinBox* yPositionSpinBox;
	// QDoubleSpinBox* zPositionSpinBox;
	// QDoubleSpinBox* xRotationSpinBox;
	// QDoubleSpinBox* yRotationSpinBox;
	// QDoubleSpinBox* zRotationSpinBox;
	// QDoubleSpinBox* xScaleSpinBox;
	// QDoubleSpinBox* yScaleSpinBox;
	// QDoubleSpinBox* zScaleSpinBox;


	void Refresh();
};

class BulletPhysicsObjectComponent : public Component {
    public: 
        enum RIGID_BODY_TYPE {DYNAMIC, STATIC, KINEMATIC};

        BulletPhysicsObjectComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

        ComponentInspector* GetInspector();

		BulletPhysicsObjectInspector* bulletPhysicsObjectInspector;

		QJsonObject ToJSON();
        static void FromJSON(QJsonObject json, Object3D* obj);		
	
    
        double mass = 0;
        RIGID_BODY_TYPE type;
        TransformComponent* transform;
        MeshFilterComponent* mesh;
        btCollisionShape* colShape;
        btRigidBody* rigidBody;
    protected:
};
}
}