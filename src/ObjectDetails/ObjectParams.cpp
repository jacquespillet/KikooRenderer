#include "ObjectParams.hpp"
#include "ObjectDetails.hpp"

#include "3DEngine/Components/Components.h"
#include "PhysicsEngines/Physics.hpp"

namespace KikooRenderer {
	/*
	Creates the first box for general object information
	*/
	ObjectDetails::ObjectDetails(CoreEngine::Object3D* _object, ObjectDetailRoot* _root) : QGroupBox("Object") {
		object = _object;
		root = _root;

		QVBoxLayout* mainLayout = new QVBoxLayout();

		QHBoxLayout* nameLayout = new QHBoxLayout();
		
		QCheckBox* isVisibleChecbBox = new QCheckBox(); 
		if (object->visible) isVisibleChecbBox->setCheckState(Qt::Checked);
		else isVisibleChecbBox->setCheckState(Qt::Unchecked);

		nameLayout->addWidget(isVisibleChecbBox);
		connect(isVisibleChecbBox, &QCheckBox::stateChanged, [this](int state) {
			object->visible = state > 0;
			emit root->InspectorModified();
		});

		QLineEdit* label = new QLineEdit(QString::fromStdString(object->name));
		nameLayout->addWidget(label);
		connect(label, &QLineEdit::textChanged, [this](const QString &text) {
			object->name = text.toStdString();
			emit root->InspectorModified();
			root->dockWidget->sceneTree->Refresh();
		});

		mainLayout->addLayout(nameLayout);

		QCheckBox* isStaticCheckBox = new QCheckBox("Static");
		mainLayout->addWidget(isStaticCheckBox);
		connect(isStaticCheckBox, &QCheckBox::stateChanged, [this](int state) {
			object->isStatic = state > 0;
			emit root->InspectorModified();
		});

		QCheckBox* castShadowCheckBox = new QCheckBox("Cast Shadow");
		castShadowCheckBox->setChecked(object->castShadow);
		mainLayout->addWidget(castShadowCheckBox);
		connect(castShadowCheckBox, &QCheckBox::stateChanged, [this](int state) {
			object->castShadow = state > 0;
			emit root->InspectorModified();
		});

		LayerController* layerController = new LayerController(object->GetLayerMask());
		mainLayout->addWidget(layerController);
		connect(layerController, &LayerController::maskChanged, this, [this](uint16_t layerMask) {
			object->SetLayerMask(layerMask);
			object->scene->triggerRefresh= true;
		});

		QPushButton* addComponentButton = new QPushButton("Add Component");
		mainLayout->addWidget(addComponentButton);
		connect(addComponentButton,  &QPushButton::clicked, this, [this](){
			QMenu* addObjectMenu = new QMenu("Add Object");
			QString physicsStr = "Physics (Bullet)";
			QString bbStr = "Bounding Box";

			addObjectMenu->addAction(physicsStr);
			addObjectMenu->addAction(bbStr);
			
			QAction* selectedItem = addObjectMenu->exec(QCursor::pos());
			if (selectedItem)
			{
				if (selectedItem->text() == physicsStr) {
					CoreEngine::BulletPhysicsObjectComponent* bulletComponent = new CoreEngine::BulletPhysicsObjectComponent(object, 1, CoreEngine::RIGID_BODY_SHAPE::CONE, CoreEngine::BODY_TYPE::RIGID);
					object->AddComponent(bulletComponent);	
					object->scene->GetSimulation()->AddObject(object);
					root->dockWidget->SetObject(object);
					// root->ClearWidgets();
					// ObjectDetails* objectDetails = new ObjectDetails(object, root);
					// root->mainLayout->addWidget(objectDetails);

					
					// std::vector<QWidget*> widgets = object->GetInspectorWidgets();
					// for(int i=0; i<widgets.size(); i++) {
					// 	if(widgets[i] != nullptr) {
					// 		root->mainLayout->addWidget(widgets[i]);
					// 	}
					// }
				}
			}	
		});
		

		setLayout(mainLayout);
	}
}