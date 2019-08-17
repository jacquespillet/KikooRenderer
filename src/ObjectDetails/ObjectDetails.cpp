#include "ObjectDetails.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer
{
	
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

		setLayout(mainLayout);
	}

	ObjectDetailRoot::ObjectDetailRoot() : QWidget() {
		mainLayout = new QVBoxLayout();	
		mainLayout->setAlignment(Qt::AlignTop);	
		setLayout(mainLayout);
	}

	void ObjectDetailRoot::ClearWidgets() {
		// if (!mainLayout)
		// 	return;
		
		// while (QLayoutItem* item = mainLayout->takeAt(0)) {
		// 	delete item->widget();
		// 	delete item;
		// }
		EmptyLayout(mainLayout);
	}

	ObjectDetailsPanel::ObjectDetailsPanel() : QDockWidget("Object Details")
	{
		setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
		rootWidget = new ObjectDetailRoot();
		rootWidget->dockWidget = this;
		setWidget(rootWidget);
	}

	void ObjectDetailsPanel::SetObject(CoreEngine::Object3D* _object) {
		rootWidget->ClearWidgets();

		ObjectDetails* objectDetails = new ObjectDetails(_object, rootWidget);
		rootWidget->mainLayout->addWidget(objectDetails);
		currentObject = _object;

		connect(rootWidget, &ObjectDetailRoot::InspectorModified, [this]() {
			currentObject->scene->triggerRefresh = true;
		});

		transformInspector = currentObject->transform->GetInspector();
		rootWidget->mainLayout->addWidget(transformInspector);

		material = (CoreEngine::MaterialComponent*)(currentObject->GetComponent("Material"));
		if (material != nullptr) {
			CoreEngine::MaterialInspector* materialInspector = material->GetInspector();
			rootWidget->mainLayout->addWidget(materialInspector);
		}



	}

	void ObjectDetailsPanel::Refresh() {
		currentObject->transform->transformInspector->Refresh();
	}

	void ObjectDetailsPanel::Clear() {
		rootWidget->ClearWidgets();
	}
}