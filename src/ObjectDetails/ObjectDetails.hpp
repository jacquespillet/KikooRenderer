#pragma once

#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"
#include "SceneTree/SceneTree.hpp"
#include "3DEngine/Components/TransformComponent.hpp"
#include "3DEngine/Components/MaterialComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {
	class MaterialComponent;
}

class View3D;
class SceneTree;
class ObjectDetailRoot;
class ObjectDetailsPanel;

//Widget for object "general" parameters
class ObjectDetails : public QGroupBox {
	Q_OBJECT
public:
	ObjectDetails(CoreEngine::Object3D* _object, ObjectDetailRoot* _root);

	CoreEngine::Object3D* object;
	ObjectDetailRoot* root;


signals:
	void InspectorModified();
};

//Root widget of the dock
class ObjectDetailRoot : public QWidget {
	Q_OBJECT
public:
	ObjectDetailRoot();
	
	ObjectDetailsPanel* dockWidget;

	QVBoxLayout* mainLayout;
	void ClearWidgets();

	QSize sizeHint() const {
		return QSize(250, 0);
	}


signals:
	void InspectorModified();
};

//Dock widget
class ObjectDetailsPanel : public QDockWidget
{
	Q_OBJECT
public:
	ObjectDetailsPanel();
	void SetObject(CoreEngine::Object3D* _object);
	void Clear();

	SceneTree* sceneTree;

	ObjectDetailRoot* rootWidget;
	CoreEngine::Object3D* currentObject;

	//Component inspectors
	CoreEngine::TransformInspector* transformInspector;
	CoreEngine::MaterialComponent * material;


	void Refresh();

signals: 
	void InspectorModified();
};
}