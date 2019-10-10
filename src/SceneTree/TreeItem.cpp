
#include "TreeItem.hpp"
#include "3DEngine/Object3D.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer 
{

void TreeItem::Refresh() {
	setText(QString::fromStdString(object3D->name));
	for (int i = 0; i < rowCount(); i++) {
		TreeItem* child = (TreeItem*)this->child(i, 0);
		child->Refresh();
	}
}

void TreeItem::Delete() {
	CoreEngine::Scene* scene = object3D->scene;
	scene->RemoveObject(object3D);

	for (int i = 0; i < rowCount(); i++) {
		TreeItem* item =(TreeItem*) child(i, 0);
		item->Delete();
	}

	if (QStandardItem* parent = QStandardItem::parent()) {
		parent->removeRow(row());
	}
	else {
		model()->removeRow(row());
	}


	scene->triggerRefresh = true;
	scene->transformWidget->Disable();
}

}