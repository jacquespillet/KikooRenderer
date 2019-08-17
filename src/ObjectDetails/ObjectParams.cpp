#include "ObjectParams.hpp"
#include "ObjectDetails.hpp"

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

		setLayout(mainLayout);
	}
}