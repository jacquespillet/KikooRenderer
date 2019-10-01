#include "ObjectDetails.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer
{


	/*
	Root widget of the inspector
	Holds all the component inspectors
	*/
	ObjectDetailRoot::ObjectDetailRoot() : QWidget() {
		mainLayout = new QVBoxLayout();	
		mainLayout->setAlignment(Qt::AlignTop);	
		setLayout(mainLayout);
	}

	/*
	Clears the layout
	*/
	void ObjectDetailRoot::ClearWidgets() {
		EmptyLayout(mainLayout);
	}

	/*
	Constructor of the right panel
	*/
	ObjectDetailsPanel::ObjectDetailsPanel() : QDockWidget("Object Details")
	{
		setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
		rootWidget = new ObjectDetailRoot();
		rootWidget->dockWidget = this;
		setWidget(rootWidget);
	}

	/*
	Set the inspected object
	@param _object : Inspected object
	*/
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

		for(int i=0; i<currentObject->components.size(); i++) {
			CoreEngine::ComponentInspector* inspector = currentObject->components[i]->GetInspector();
			if(inspector !=nullptr) {
				rootWidget->mainLayout->addWidget(inspector);
			}
		}
	}

	void ObjectDetailsPanel::Refresh() {
		currentObject->transform->transformInspector->Refresh();
	}

	/*
	Clears all the widgets of the inspector
	*/
	void ObjectDetailsPanel::Clear() {
		rootWidget->ClearWidgets();
	}
}