#include "SceneTree.hpp"

// #include "3DEngine/BaseObjects.hpp"

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

SceneTree::SceneTree() : QDockWidget("Scene")
{
    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    
    tree = new SceneTreeView();
    tree->setObjectName("DirView");
    tree->setUniformRowHeights(true);
    tree->setHeaderHidden(true);
	tree->sceneTree = this;
    
    model = new QStandardItemModel(0, 1);
    tree->setModel( model ); 
	tree->model = model;

	connect(tree->model, SIGNAL(itemChanged(QStandardItem*)),
		this, SLOT(OnItemChanged(QStandardItem*)));


    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    setWidget(tree);
}

void SceneTree::OnItemChanged(QStandardItem* item) {
	TreeItem* treeItem = (TreeItem*)item;
	treeItem->object3D->name = treeItem->text().toStdString();
	treeItem->object3D->scene->objectDetailsPanel->Refresh();
}

void SceneTree::Refresh() {
	for (int i = 0; i < model->rowCount(); i++) {
		TreeItem* item = (TreeItem*) model->item(i, 0);
		item->Refresh();
	}
}

//Handles menu Creation
void SceneTree::ShowContextMenu(const QPoint& pos, bool fromMainWindow)
{   
    QString cubeStr = "Cube";
    QString sphereStr = "Sphere";
    QString circleStr = "Circle";
    QString quadStr = "Quad";
	QString emptyStr = "Empty";
	QString dirLightStr = "Directional Light";
	QString pointLightStr = "Point Light";
	QString spotLightStr = "Spot Light";

	QPoint menuPos;
	if(!fromMainWindow) menuPos = tree->mapToGlobal(pos);
	else menuPos = tree->mapFromParent(pos);

    QMenu myMenu;

    QMenu* addObjectMenu = new QMenu("Add Object");
    addObjectMenu->addAction(cubeStr);
    addObjectMenu->addAction(sphereStr);
    addObjectMenu->addAction(circleStr);
    addObjectMenu->addAction(quadStr);
    addObjectMenu->addAction(emptyStr);

	addObjectMenu->addSeparator();

    addObjectMenu->addAction(dirLightStr);
    addObjectMenu->addAction(pointLightStr);
    addObjectMenu->addAction(spotLightStr);


    myMenu.addMenu(addObjectMenu);
    QAction* selectedItem = myMenu.exec(menuPos);


    if (selectedItem)
    {
		QModelIndexList	selectedIndexes = tree->selectionModel()->selectedIndexes();
		
		CoreEngine::Object3D* objectToAdd = nullptr;
		QString name = QString("New ") + selectedItem->text();
		if (selectedItem->text() == cubeStr) objectToAdd = CoreEngine::GetCube(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == sphereStr) objectToAdd = CoreEngine::GetSphere(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == circleStr) objectToAdd = CoreEngine::GetCircle(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == quadStr) objectToAdd = CoreEngine::GetQuad(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == emptyStr) objectToAdd = new CoreEngine::Object3D(name.toStdString(), view3D->view3DGL->scene);
		if (selectedItem->text() == dirLightStr) objectToAdd = CoreEngine::GetDirectionalLight(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == pointLightStr) objectToAdd = CoreEngine::GetPointLight(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == spotLightStr) objectToAdd = CoreEngine::GetSpotLight(view3D->view3DGL->scene, name.toStdString(), glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1), 90);
		
		if (selectedIndexes.size() > 0) {
			TreeItem* parentItem = (TreeItem*)model->itemFromIndex(selectedIndexes[0]);
			parentItem->object3D->AddObject(objectToAdd);

			TreeItem* itemToAdd = new TreeItem(QString(name));
			itemToAdd->object3D = objectToAdd;
			parentItem->appendRow(itemToAdd);
		}
		else {
			std::string finalName = view3D->view3DGL->scene->AddObject(objectToAdd);
			name = QString::fromStdString(finalName);
			TreeItem* itemToAdd = new TreeItem(name);

			itemToAdd->object3D = objectToAdd;
			model->appendRow(itemToAdd);
		}

		view3D->view3DGL->scene->triggerRefresh = true;
    }
}


void SceneTreeView::mousePressEvent(QMouseEvent *event) {
	QModelIndex index = indexAt(event->pos());
	if (index.isValid()) {
		bool selected = selectionModel()->isSelected(index);
		QTreeView::mousePressEvent(event);
		
		TreeItem* item = (TreeItem*)model->itemFromIndex(index);
		sceneTree->view3D->view3DGL->scene->AddObjectToSelection(true, item->object3D);
		sceneTree->view3D->view3DGL->scene->triggerRefresh = true;
	}
	else {
		selectionModel()->clearSelection();
		sceneTree->view3D->view3DGL->scene->ClearSelection();
		sceneTree->view3D->view3DGL->scene->triggerRefresh = true;
	}
}

void SceneTree::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Delete) {
		QModelIndexList	selectedIndexes = tree->selectionModel()->selectedIndexes();
		if (selectedIndexes.size() > 0) {
			TreeItem* parentItem = (TreeItem*)model->itemFromIndex(selectedIndexes[0]);
			parentItem->Delete();

		}
	}
}

}