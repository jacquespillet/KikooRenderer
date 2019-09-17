#pragma once

#include "Util/Common.h"
#include "ObjectParams.hpp"
#include "3DEngine/Object3D.hpp"
#include "SceneTree/SceneTree.hpp"
#include "3DEngine/Components/TransformComponent.hpp"
#include "3DEngine/Components/MaterialComponent.hpp"
#include "3DEngine/Components/MeshFilterComponent.hpp"
#include "3DEngine/Components/LightComponent.hpp"

namespace KikooRenderer {
namespace CoreEngine {
	class MaterialComponent;
}

class View3D;
class SceneTree;
class ObjectDetailRoot;
class ObjectDetailsPanel;


/*
Main widget for the dockWidget
*/
class ObjectDetailRoot : public QWidget {
	Q_OBJECT
public:
	ObjectDetailRoot();
	
	ObjectDetailsPanel* dockWidget;

	QVBoxLayout* mainLayout;
	void ClearWidgets();

	QSize sizeHint() const {
		return QSize(width(), 0);
	}


signals:
	void InspectorModified();
};

/*
DockWidget for the object inspector
*/
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