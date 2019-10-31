#pragma once 
#include "Util/Common.h"
#include "3DEngine/Components/Component.hpp"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class Scene;
class MeshFilterComponent;
class TransformComponent;
class BoundingBoxComponent;
class BulletPhysicsObjectComponent;

enum RIGID_BODY_SHAPE {BOX, CONE, SPHERE, CAPSULE, CYLINDER, MESH, HEIGHTFIELD};
enum BODY_TYPE {RIGID, SOFT, DEFORMABLE};
class BulletPhysicsObjectInspector : public ComponentInspector {
	Q_OBJECT
public:
	BulletPhysicsObjectInspector(BulletPhysicsObjectComponent* BulletPhysicsObjectComponent);
	BulletPhysicsObjectComponent* bulletPhysicsObjectComponent;
	Scene* scene;

    QGroupBox* rigidBodySettingsGroupBox;
    QGroupBox* softBodySettingsGroupBox;

	void Refresh();
};

class BulletPhysicsObjectComponent : public Component {
    public: 
        enum RIGID_BODY_TYPE {DYNAMIC, STATIC, KINEMATIC};

        BulletPhysicsObjectComponent(Object3D* object, double mass, RIGID_BODY_SHAPE shape, BODY_TYPE bodyType = BODY_TYPE::RIGID, std::vector<int> staticNodeIndices = std::vector<int>());
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

        void Init();

        ComponentInspector* GetInspector();

		BulletPhysicsObjectInspector* bulletPhysicsObjectInspector;

		QJsonObject ToJSON();
        static void FromJSON(QJsonObject json, Object3D* obj);		

        bool showBounds = false;

        std::vector<int> staticNodeIndices;

        Object3D* meshColliderObject;
    
        double mass = 0;
        double margin = 0.25;
        // RIGID_BODY_TYPE type;
        RIGID_BODY_SHAPE shape;
        BODY_TYPE bodyType;

        TransformComponent* transform;
        MeshFilterComponent* mesh;
        BoundingBoxComponent* bb;

        btCollisionShape* colShape;
        btRigidBody* rigidBody;
        btSoftBody* softBody;
    protected:
};
}
}