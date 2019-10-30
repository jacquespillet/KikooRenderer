#include "BulletComponent.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"
#include "3DEngine/Components/BoundingComponent.hpp"


namespace KikooRenderer {
namespace CoreEngine {

/*
** ___________________________________________________________________________
** /!\ These parameters cannot be changed if the simulation is already running
** 	   --> They must be set before pressing play
** ___________________________________________________________________________
*/

BulletPhysicsObjectInspector::BulletPhysicsObjectInspector(BulletPhysicsObjectComponent* _bulletPhysicsObjectComponent) : ComponentInspector("Bullet Physics Object", _bulletPhysicsObjectComponent)  {
    setLocale(QLocale("a")); 

	this->bulletPhysicsObjectComponent = _bulletPhysicsObjectComponent;
	Object3D* object = bulletPhysicsObjectComponent->object3D;
	scene = object->scene;
	QVBoxLayout* mainLayout = new QVBoxLayout();

	
	///1.__________________________________________________________________________________________________________
	//Shape size inspectors
	
	//////1.1__________________________________________________________________________________________________________
	/////Box
	glm::vec3 min, max;
	bulletPhysicsObjectComponent->bb->GetLocalBounds(&min, &max);
	min *= bulletPhysicsObjectComponent->object3D->transform->scale;
	max *= bulletPhysicsObjectComponent->object3D->transform->scale;

	float sizeX = max.x - min.x;
	float sizeY = max.y - min.y;
	float sizeZ = max.z - min.z;	
	Vector3Inspector* boxSizeInspector = new Vector3Inspector("Box Collider Size", glm::vec3(sizeX, sizeY, sizeZ));
	connect(boxSizeInspector, &Vector3Inspector::Modified, this, [this](glm::vec3 vector) {
		bulletPhysicsObjectComponent->colShape =  new btBoxShape(btVector3(vector.x * 0.5,vector.y * 0.5,vector.z * 0.5));
		bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
        scene->triggerRefresh = true;
	});
	boxSizeInspector->setVisible(RIGID_BODY_SHAPE::BOX == bulletPhysicsObjectComponent->shape);

	//////1.2__________________________________________________________________________________________________________
	/////Cone
	QWidget* coneShapeSizeWidget = new QWidget();
	QHBoxLayout* coneShapeSizeLayout = new QHBoxLayout();
	coneShapeSizeWidget->setLayout(coneShapeSizeLayout);
	
	float coneRadiusScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y+ bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.3333333;
	CustomSlider* coneRadiusSlider = new CustomSlider(0.0f, 10.0f, 0.01, "cone Radius", coneRadiusScale * 0.25);
    QObject::connect(coneRadiusSlider, &CustomSlider::Modified, [this](double val) {
		btConeShapeZ* coneShape = (btConeShapeZ*) bulletPhysicsObjectComponent->colShape;
		float height = coneShape->getHeight();
		
		bulletPhysicsObjectComponent->colShape =  new btConeShapeZ(btScalar(val), height);		
		bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
        scene->triggerRefresh = true;
	});	
	coneShapeSizeLayout->addLayout(coneRadiusSlider);

	float coneHeightScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y+ bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.3333333;
	CustomSlider* coneHeightSlider = new CustomSlider(0.0f, 10.0f, 0.01, "cone Height", coneHeightScale);
    QObject::connect(coneHeightSlider, &CustomSlider::Modified, [this](double val) {
		btConeShapeZ* coneShape = (btConeShapeZ*) bulletPhysicsObjectComponent->colShape;
		float radius = coneShape->getRadius();
		
		bulletPhysicsObjectComponent->colShape =  new btConeShapeZ(radius, btScalar(val));		
		bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
        scene->triggerRefresh = true;
	});	
	coneShapeSizeLayout->addLayout(coneHeightSlider);
	coneShapeSizeWidget->setVisible(RIGID_BODY_SHAPE::CONE == bulletPhysicsObjectComponent->shape);	

	//////1.3__________________________________________________________________________________________________________
	/////Sphere
	float sphereRadiusScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y+ bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.3333333;
	CustomSlider* sphereRadiusSlider = new CustomSlider(0.0f, 10.0f, 0.01, "Sphere Radius", sphereRadiusScale);
    QObject::connect(sphereRadiusSlider, &CustomSlider::Modified, [this](double val) {
		bulletPhysicsObjectComponent->colShape =  new btSphereShape(btScalar(val));
		bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
        scene->triggerRefresh = true;
	});
	sphereRadiusSlider->SetVisible(RIGID_BODY_SHAPE::SPHERE == bulletPhysicsObjectComponent->shape);


	//////1.4__________________________________________________________________________________________________________
	/////Capsule
	QWidget* capsuleShapeSizeWidget = new QWidget();
	QHBoxLayout* capsuleShapeSizeLayout = new QHBoxLayout();
	capsuleShapeSizeWidget->setLayout(capsuleShapeSizeLayout);
	
	float capsuleRadiusScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y+ bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.3333333;
	CustomSlider* capsuleRadiusSlider = new CustomSlider(0.0f, 10.0f, 0.01, "capsule Radius", capsuleRadiusScale);
    QObject::connect(capsuleRadiusSlider, &CustomSlider::Modified, [this](double val) {
		btCapsuleShape* coneShape = (btCapsuleShape*) bulletPhysicsObjectComponent->colShape;
		float height = coneShape->getHalfHeight() * 2;

		bulletPhysicsObjectComponent->colShape =  new btCapsuleShape(btScalar(val), height);	
		bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
        scene->triggerRefresh = true;
	});	
	capsuleShapeSizeLayout->addLayout(capsuleRadiusSlider);

	float capsuleHeightScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y+ bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.3333333;
	CustomSlider* capsuleHeightSlider = new CustomSlider(0.0f, 10.0f, 0.01, "capsule Height", capsuleHeightScale);
    QObject::connect(capsuleHeightSlider, &CustomSlider::Modified, [this](double val) {
		btCapsuleShape* coneShape = (btCapsuleShape*) bulletPhysicsObjectComponent->colShape;
		float radius = coneShape->getRadius();

		bulletPhysicsObjectComponent->colShape =  new btCapsuleShape(radius, btScalar(val));	
		bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
        scene->triggerRefresh = true;
	});	
	capsuleShapeSizeLayout->addLayout(capsuleHeightSlider);
	capsuleShapeSizeWidget->setVisible(RIGID_BODY_SHAPE::CAPSULE == bulletPhysicsObjectComponent->shape);

	///__________________________________________________________________________________________________________


	///2.__________________________________________________________________________________________________________
	//Set the SHAPE (W/ REALTIME WIREFRAME VISUALIZATION)
	QHBoxLayout* shapeLayout = new QHBoxLayout();
	QComboBox* shapeList = new QComboBox();
	QLabel* shapeLabel = new QLabel("Collider Shape");
	shapeList->addItem("Box");
	shapeList->addItem("Cone");
	shapeList->addItem("Sphere");
	shapeList->addItem("Capsule");
	shapeList->addItem("Cylinder");
	shapeList->addItem("Mesh");
	shapeList->addItem("HeightField");

	shapeList->setCurrentIndex(bulletPhysicsObjectComponent->shape);

	shapeLayout->addWidget(shapeLabel);
	shapeLayout->addWidget(shapeList);
	mainLayout->addLayout(shapeLayout);

	connect(shapeList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this, boxSizeInspector, coneShapeSizeWidget, sphereRadiusSlider, capsuleShapeSizeWidget](int index) {
		if(RIGID_BODY_SHAPE::BOX == (RIGID_BODY_SHAPE) index) {
			glm::vec3 min, max;
			bulletPhysicsObjectComponent->bb->GetLocalBounds(&min, &max);
			min *= bulletPhysicsObjectComponent->object3D->transform->scale;
			max *= bulletPhysicsObjectComponent->object3D->transform->scale;

			float sizeX = max.x - min.x;
			float sizeY = max.y - min.y;
			float sizeZ = max.z - min.z;
			
			bulletPhysicsObjectComponent->colShape =  new btBoxShape(btVector3(sizeX * 0.5,sizeY * 0.5,sizeZ * 0.5));
			boxSizeInspector->Setvalue(sizeX, sizeY, sizeZ, 1);
			
			bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
			bulletPhysicsObjectComponent->shape = (RIGID_BODY_SHAPE)index;

			boxSizeInspector->setVisible(true);			
			coneShapeSizeWidget->setVisible(false);	
			sphereRadiusSlider->SetVisible(false);	
			capsuleShapeSizeWidget->setVisible(false);
		} else if(RIGID_BODY_SHAPE::CONE == (RIGID_BODY_SHAPE) index) {
			float radiusScale = 0.25 * (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y) * 0.5;
			float height = 1 * bulletPhysicsObjectComponent->object3D->transform->scale.z;
			std::cout << "Values " << radiusScale << "  " << height << std::endl;

			bulletPhysicsObjectComponent->colShape =  new btConeShapeZ(radiusScale, height);

			bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
			bulletPhysicsObjectComponent->shape = (RIGID_BODY_SHAPE)index;
			
			boxSizeInspector->setVisible(false);			
			coneShapeSizeWidget->setVisible(true);	
			sphereRadiusSlider->SetVisible(false);	
			capsuleShapeSizeWidget->setVisible(false);
		} else if(RIGID_BODY_SHAPE::SPHERE == (RIGID_BODY_SHAPE) index) {
			float radiusScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.y+ bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.3333333;
			bulletPhysicsObjectComponent->colShape =  new btSphereShape(btScalar(radiusScale));
			bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
			bulletPhysicsObjectComponent->shape = (RIGID_BODY_SHAPE)index;

			boxSizeInspector->setVisible(false);			
			coneShapeSizeWidget->setVisible(false);	
			sphereRadiusSlider->SetVisible(true);	
			capsuleShapeSizeWidget->setVisible(false);
		}  else if(RIGID_BODY_SHAPE::CAPSULE == (RIGID_BODY_SHAPE) index) {
			float radiusScale = (bulletPhysicsObjectComponent->object3D->transform->scale.x + bulletPhysicsObjectComponent->object3D->transform->scale.z) * 0.5;
			float height = 2 * bulletPhysicsObjectComponent->object3D->transform->scale.z;
			bulletPhysicsObjectComponent->colShape =  new btCapsuleShape(btScalar(radiusScale), btScalar(height));
			bulletPhysicsObjectComponent->rigidBody->setCollisionShape(bulletPhysicsObjectComponent->colShape);
			bulletPhysicsObjectComponent->shape = (RIGID_BODY_SHAPE)index;

			boxSizeInspector->setVisible(false);			
			coneShapeSizeWidget->setVisible(false);	
			sphereRadiusSlider->SetVisible(false);
			capsuleShapeSizeWidget->setVisible(true);				
		} 
		//else if(RIGID_BODY_SHAPE::CYLINDER == (RIGID_BODY_SHAPE) index) {
		// 	//https://pybullet.org/Bullet/BulletFull/classbtCylinderShape.html#a168602b231f1a308281a65fdb8d4f93d
		// } else if(RIGID_BODY_SHAPE::MESH == (RIGID_BODY_SHAPE) index) {
		// 	//https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=4513	
		// } else if(RIGID_BODY_SHAPE::HEIGHTFIELD == (RIGID_BODY_SHAPE) index) {
		// 	//https://pybullet.org/Bullet/BulletFull/classbtHeightfieldTerrainShape.html
		// }
        scene->triggerRefresh = true;
	});
	///__________________________________________________________________________________________________________
	
	///3.__________________________________________________________________________________________________________
	///Enable / Disable visualisation of the bounding box
	QCheckBox* rigidBodyVisibleCheckBox = new QCheckBox("Show Bounds");
    mainLayout->addWidget(rigidBodyVisibleCheckBox);
	connect(rigidBodyVisibleCheckBox, &QCheckBox::stateChanged, this, [this](int state ) {
		bulletPhysicsObjectComponent->showBounds = state > 0;
        scene->triggerRefresh = true;
	});

	mainLayout->addWidget(boxSizeInspector);
	mainLayout->addWidget(coneShapeSizeWidget);
	mainLayout->addLayout(sphereRadiusSlider);
	mainLayout->addWidget(capsuleShapeSizeWidget);

	///__________________________________________________________________________________________________________

	///4.__________________________________________________________________________________________________________
    ///Sets the mass of the physical object
	CustomSlider* massSlider = new CustomSlider(0.0f, 3.0f, 0.01, "Mass", bulletPhysicsObjectComponent->mass);
    mainLayout->addLayout(massSlider);
    QObject::connect(massSlider, &CustomSlider::Modified, [this](double val) {
        bulletPhysicsObjectComponent->mass = val;

		bulletPhysicsObjectComponent->object3D->scene->GetSimulation()->RemoveObject(bulletPhysicsObjectComponent->object3D);

		btVector3 inertia;
		bulletPhysicsObjectComponent->rigidBody->getCollisionShape()->calculateLocalInertia( bulletPhysicsObjectComponent->mass, inertia );
		bulletPhysicsObjectComponent->rigidBody->setMassProps(bulletPhysicsObjectComponent->mass, inertia);
		
		bulletPhysicsObjectComponent->object3D->scene->GetSimulation()->AddObject(bulletPhysicsObjectComponent->object3D);

        scene->triggerRefresh = true;
    });
	///__________________________________________________________________________________________________________
	
	//Set the margin
	
	//Set all theses guys
	// Friction
	// RollingFriction
	// SpinningFriction
	// AnisotropicFriction

	//If soft or deformable
		//Material settings
		//Deformation settings
		//Pose Saving settings
		//Wind settings
		//Contact hardness settings


    
	setLayout(mainLayout);
}

BulletPhysicsObjectComponent::BulletPhysicsObjectComponent(Object3D* object, double _mass, RIGID_BODY_SHAPE _shape, BODY_TYPE _bodyType, std::vector<int> staticNodeIndices) : Component("Bullet", object) {
	this->mass = _mass;
	this->bb = object->GetComponent<BoundingBoxComponent>();
	this->mesh = object->GetComponent<MeshFilterComponent>();
	this->transform = object->transform;
	this->shape = _shape;
	this->bodyType = _bodyType;

	if(_bodyType == BODY_TYPE::RIGID) {
		if(_shape == RIGID_BODY_SHAPE::BOX) {
			glm::vec3 min, max;
			bb->GetLocalBounds(&min, &max);
			min *= object->transform->scale;
			max *= object->transform->scale;

			float sizeX = std::max(max.x - min.x, 0.1f);
			float sizeY = std::max(max.y - min.y, 0.1f);
			float sizeZ = std::max(max.z - min.z, 0.1f);
			
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
		
		rigidBody->setFriction(1.f);
		rigidBody->setRollingFriction(0.01);
		rigidBody->setSpinningFriction(0.01);
		rigidBody->setAnisotropicFriction(colShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	} else if(_bodyType == BODY_TYPE::SOFT) {
		std::vector<btScalar> vertices(mesh->vertices.size() * 3);
		glm::mat4 modelMatrix = transform->GetModelMatrix();
		transform->position = glm::vec3(0);
		transform->rotation = glm::vec3(0);
		transform->scale = glm::vec3(1);

		for(int i=0; i<mesh->vertices.size(); i++) {
			int j=i*3;
			glm::vec4 finalPos = modelMatrix * glm::vec4(mesh->vertices[i].position, 1);
			vertices[j]   =  (btScalar)(finalPos.x);
			vertices[j+1] = (btScalar)(finalPos.y);
			vertices[j+2] = (btScalar)(finalPos.z);
		}		
		btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)object3D->scene->GetSimulation()->dynamicsWorld; 

		softBody = btSoftBodyHelpers::CreateFromTriMesh (world->getWorldInfo(),
			&vertices[0], 
			&mesh->GetTriangles()[0], 
			mesh->GetTriangles().size() / 3
		);

		softBody->generateBendingConstraints(2, softBody->m_materials[0]);

		softBody->getCollisionShape()->setMargin(0.2);	
		softBody->m_cfg.piterations = 200;
		softBody->m_cfg.citerations = 200;
		softBody->m_cfg.diterations = 200;
		// softBody->m_cfg.viterations = 200;
		
		//MATERIAL SETTINGS
		{
			// softBody->m_materials[0]->m_kLST = 0.01;  // Linear stiffness coefficient [0,1]
			// softBody->m_materials[0]->m_kAST = 0.01;  // Area/Angular stiffness coefficient [0,1]
			// softBody->m_materials[0]->m_kVST = 0.01;  // Volume stiffness coefficient [0,1]
		}
		
		//SAVE POSE
		{
			softBody->m_cfg.kMT = 0.2;
			softBody->setPose(true, true);
		}

		//DEFORMATION
		{
			// softBody->m_cfg.kDF = 0.5;
			// softBody->m_cfg.kDP = 0.001;  // fun factor...
			// softBody->m_cfg.kPR = 2000;		
			// softBody->m_cfg.kVC = 20;
		}

		//WIND
		{
			// softBody->m_cfg.aeromodel = btSoftBody::eAeroModel::V_TwoSided;
			// softBody->setTotalMass(0.0001);
			// softBody->addForce(btVector3(0, 0.01, 0), 0);
		}

		//OTHER PARAMS
		{
			// softBody->m_cfg.kPR = 0.01;
			// softBody->m_cfg.kLF = 0;
			// softBody->m_cfg.kDG = 0;
			// softBody->m_cfg.kVC = 1;
			// softBody->m_cfg.kDF = 0.5;
			// softBody->m_cfg.kCHR = 1;
			// btScalar kVCF;
		}
		
		softBody->randomizeConstraints();		
		softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
		
		softBody->setTotalMass(mass);


		//Other possible settings
		// btScalar kKHR;              // Kinetic contacts hardness [0,1]
		// btScalar kSHR;              // Soft contacts hardness [0,1]
		// btScalar kAHR;              // Anchors hardness [0,1]
		// btScalar kSRHR_CL;          // Soft vs rigid hardness [0,1] (cluster only)
		// btScalar kSKHR_CL;          // Soft vs kinetic hardness [0,1] (cluster only)
		// btScalar kSSHR_CL;          // Soft vs soft hardness [0,1] (cluster only)
		// btScalar kSR_SPLT_CL;       // Soft vs rigid impulse split [0,1] (cluster only)
		// btScalar kSK_SPLT_CL;       // Soft vs rigid impulse split [0,1] (cluster only)
		// btScalar kSS_SPLT_CL;       // Soft vs rigid impulse split [0,1] (cluster only)
		// btScalar maxvolume;         // Maximum volume ratio for pose
		// btScalar timescale;         // Time scale


		for(int i=0; i<staticNodeIndices.size(); i++) {
			softBody->setMass(staticNodeIndices[i], 0);
		}
	} else if(_bodyType == BODY_TYPE::DEFORMABLE) {
		std::vector<btScalar> vertices(mesh->vertices.size() * 3);
		glm::mat4 modelMatrix = transform->GetModelMatrix();
		transform->position = glm::vec3(0);
		transform->rotation = glm::vec3(0);
		transform->scale = glm::vec3(1);

		for(int i=0; i<mesh->vertices.size(); i++) {
			int j=i*3;
			glm::vec4 finalPos = modelMatrix * glm::vec4(mesh->vertices[i].position, 1);
			vertices[j]   =  (btScalar)(finalPos.x);
			vertices[j+1] = (btScalar)(finalPos.y);
			vertices[j+2] = (btScalar)(finalPos.z);
		}		
		btDeformableMultiBodyDynamicsWorld* world = (btDeformableMultiBodyDynamicsWorld*)object3D->scene->GetSimulation()->dynamicsWorld; 
		if(world != nullptr) {
			softBody = btSoftBodyHelpers::CreateFromTriMesh (world->getWorldInfo(),
				&vertices[0], 
				&mesh->GetTriangles()[0], 
				mesh->GetTriangles().size() / 3
			);

			softBody->getCollisionShape()->setMargin(0.1);
			softBody->generateBendingConstraints(2);
			softBody->setTotalMass(1);
			softBody->m_cfg.kKHR = 1; // collision hardness with kinematic objects
			softBody->m_cfg.kCHR = 1; // collision hardness with rigid body
			softBody->m_cfg.kDF = 0.1;
			softBody->m_cfg.collisions = btSoftBody::fCollision::SDF_RD;
			softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_DD;
			// world->addSoftBody(softBody);
			
			//elastic stiffness & damping stiffness
			btDeformableMassSpringForce* mass_spring2 = new btDeformableMassSpringForce(0.1,0.1, false);
			world->addForce(softBody, mass_spring2);
			// m_forces.push_back(mass_spring2);
			
			btVector3 gravity = btVector3(0, -10, 0);
			btDeformableGravityForce* gravity_force2 =  new btDeformableGravityForce(gravity);
			world->addForce(softBody, gravity_force2);
			// m_forces.push_back(gravity_force2);			

			// softBody->generateBendingConstraints(2, softBody->m_materials[0]);

			// softBody->getCollisionShape()->setMargin(0.2);	
			// softBody->m_cfg.piterations = 2;
			// softBody->m_cfg.citerations = 2;
			// softBody->m_cfg.diterations = 2;
			// // softBody->m_cfg.viterations = 200;
			
			// //MATERIAL SETTINGS
			// {
			// 	// softBody->m_materials[0]->m_kLST = 0.01;  // Linear stiffness coefficient [0,1]
			// 	// softBody->m_materials[0]->m_kAST = 0.01;  // Area/Angular stiffness coefficient [0,1]
			// 	// softBody->m_materials[0]->m_kVST = 0.01;  // Volume stiffness coefficient [0,1]
			// }
			
			// //SAVE POSE
			// {
				// softBody->m_cfg.kMT = 1;
				// softBody->setPose(false, true);
			// }

			// //DEFORMATION
			// {
			// 	// softBody->m_cfg.kDF = 0.5;
			// 	// softBody->m_cfg.kDP = 0.001;  // fun factor...
			// 	// softBody->m_cfg.kPR = 2000;	 //Pressure coefficient [-inf,+inf]	
			// 	// softBody->m_cfg.kVC = 20;
			// }

			// //WIND
			// {
			// 	// softBody->m_cfg.aeromodel = btSoftBody::eAeroModel::V_TwoSided;
			// 	// softBody->setTotalMass(0.0001);
			// 	// softBody->addForce(btVector3(0, 0.01, 0), 0);
			// }

			// //CONTACT HARDNESS
			// {
			// 	softBody->m_cfg.kCHR = 1;              // Rigid contacts hardness [0,1]
			// 	softBody->m_cfg.kKHR = 1;              // Kinetic contacts hardness [0,1]
			// 	softBody->m_cfg.kSHR = 1;              // Soft contacts hardness [0,1]
			// 	softBody->m_cfg.kAHR = 1;              // Anchors hardness [0,1]
			// 	softBody->m_cfg.kSRHR_CL = 1;          // Soft vs rigid hardness [0,1] (cluster only)
			// 	softBody->m_cfg.kSKHR_CL = 1;          // Soft vs kinetic hardness [0,1] (cluster only)
			// 	softBody->m_cfg.kSSHR_CL = 1;          // Soft vs soft hardness [0,1] (cluster only)
			// }

			// //OTHER PARAMS
			// {
			// 	// softBody->m_cfg.kLF = 0;  // Lift coefficient [0,+inf]
			// 	// softBody->m_cfg.kDG = 0;  // Drag coefficient [0,+inf]
			// 	// btScalar kVCF;
			// }
			
			// softBody->randomizeConstraints();		
			// softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
			
			// softBody->setTotalMass(150);

			// //Other possible settings
			// // btScalar kSR_SPLT_CL;       // Soft vs rigid impulse split [0,1] (cluster only)
			// // btScalar kSK_SPLT_CL;       // Soft vs rigid impulse split [0,1] (cluster only)
			// // btScalar kSS_SPLT_CL;       // Soft vs rigid impulse split [0,1] (cluster only)
			// // btScalar maxvolume;         // Maximum volume ratio for pose
			// // btScalar timescale;         // Time scale


			
			btDeformableGravityForce* gravity_force =  new btDeformableGravityForce(btVector3(0, -10, 0));
			world->addForce(softBody, gravity_force);
		}

	}
	
}

void BulletPhysicsObjectComponent::OnStart() {

}

void BulletPhysicsObjectComponent::OnEnable() {

}

void BulletPhysicsObjectComponent::OnUpdate() {
	if(object3D->transform->hasChanged) {
		//UPDATE THE BODY POSITION
	}
}

void BulletPhysicsObjectComponent::OnRender() {
	if(showBounds) {
		if(shape == RIGID_BODY_SHAPE::BOX) {
			glm::vec3 position = object3D->transform->position;
			glm::vec3 rotation = object3D->transform->rotation;
			btBoxShape* boxShape = (btBoxShape*) colShape;
			btVector3 halfSize = boxShape->getHalfExtentsWithMargin ();
			glm::vec3 scale    =  glm::vec3(halfSize.x() * 2, halfSize.y() * 2, halfSize.z() * 2);

			object3D->scene->drawImmediate.DrawWireBox(position, rotation, scale, glm::vec4(1, 0, 0, 1));
		} else if(shape == RIGID_BODY_SHAPE::SPHERE) {
			glm::vec3 position = object3D->transform->position;
			glm::vec3 rotation = object3D->transform->rotation;
			btSphereShape* sphereShape = (btSphereShape*) colShape;
			float radius = sphereShape->getRadius();
			glm::vec3 scale    =  glm::vec3(radius);
			object3D->scene->drawImmediate.DrawWireSphere(position, rotation, scale, glm::vec4(1, 0, 0, 1));
		} else if(shape == RIGID_BODY_SHAPE::CONE) {
			glm::vec3 position = object3D->transform->position;
			glm::vec3 rotation = object3D->transform->rotation;
			btConeShapeZ* coneShape = (btConeShapeZ*) colShape;
			float radius = coneShape->getRadius();
			float height = coneShape->getHeight();
			glm::vec3 scale =  glm::vec3(radius / 0.25, radius / 0.25, height);
			object3D->scene->drawImmediate.DrawWireCone(position, rotation, scale, glm::vec4(1, 0, 0, 1));
		} else if(shape == RIGID_BODY_SHAPE::CAPSULE) {
			glm::vec3 position = object3D->transform->position;
			glm::vec3 rotation = object3D->transform->rotation;
			btCapsuleShape* coneShape = (btCapsuleShape*) colShape;
			float radius = coneShape->getRadius();
			float height = coneShape->getHalfHeight() * 2;
			glm::vec3 scale =  glm::vec3(radius, height, radius);
			object3D->scene->drawImmediate.DrawWireCapsule(position, rotation, scale, glm::vec4(1, 0, 0, 1));
		}
	}
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