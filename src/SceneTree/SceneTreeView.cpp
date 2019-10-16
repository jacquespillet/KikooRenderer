

#include "SceneTreeView.hpp"
#include "TreeItem.hpp"
#include "SceneTree.hpp"
#include "3DEngine/CameraControllers/CameraController.hpp"

namespace KikooRenderer 
{

void SceneTreeView::mousePressEvent(QMouseEvent *event) {
	QModelIndex index = indexAt(event->pos());
	if (index.isValid()) {
		bool selected = selectionModel()->isSelected(index);
		QTreeView::mousePressEvent(event);
		
		TreeItem* item = (TreeItem*)model->itemFromIndex(index);
		sceneTree->view3D->view3DGL->scene->AddObjectToSelection(!(event->modifiers() & Qt::ControlModifier), item->object3D);
		sceneTree->view3D->view3DGL->scene->triggerRefresh = true;
	}
	else {
		selectionModel()->clearSelection();
		sceneTree->view3D->view3DGL->scene->ClearSelection();
		sceneTree->view3D->view3DGL->scene->triggerRefresh = true;
	}
}

void SceneTreeView::mouseDoubleClickEvent(QMouseEvent *event) {
	sceneTree->view3D->view3DGL->scene->camera->GetCameraController()->MoveToFocus();
}

}