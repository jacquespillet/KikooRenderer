#pragma once

#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"

namespace KikooRenderer {

	class View3D;

	class ObjectDetails : public QWidget {
	public: 
		ObjectDetails(CoreEngine::Object3D* object) : QWidget() {
			QHBoxLayout* mainLayout = new QHBoxLayout();

			QLabel* label = new QLabel(QString::fromStdString(object->name));
			QCheckBox* checkBox = new QCheckBox();
			mainLayout->addWidget(label);
			mainLayout->addWidget(checkBox);

			setLayout(mainLayout);
		}
	};


	class ObjectDetailsPanel : public QDockWidget
	{
		Q_OBJECT
	public:
		ObjectDetailsPanel();

		QWidget* mainWidget;
		QVBoxLayout* mainLayout;
	};
}