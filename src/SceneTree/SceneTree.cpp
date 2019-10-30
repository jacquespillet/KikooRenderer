#include "SceneTree.hpp"
#include "SceneTreeView.hpp"
#include "TreeItem.hpp"

#include "3DEngine/BaseObjects.hpp"

namespace KikooRenderer 
{


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

void SceneTree::AddObject(CoreEngine::Object3D* objectToAdd, CoreEngine::Object3D* parent) {
	QString name = QString::fromStdString(objectToAdd->name);
	TreeItem* itemToAdd = new TreeItem(name);
	itemToAdd->object3D = objectToAdd;
	model->appendRow(itemToAdd);
}

//Handles menu Creation
void SceneTree::ShowContextMenu(const QPoint& pos, bool fromMainWindow)
{   
    QString cubeStr = "Cube";
    QString sphereStr = "Sphere";
    QString circleStr = "Circle";
    QString quadStr = "Quad";
    QString capsuleStr = "Capsule";
    QString coneStr = "Cone";
    QString cylinderStr = "Cylinder";
	QString emptyStr = "Empty";

	QString dirLightStr = "Directional Light";
	QString pointLightStr = "Point Light";
	QString spotLightStr = "Spot Light";

	QString rabbitStr = "Rabbit";
	QString buddhaStr = "Buddha";
	QString dragonStr = "Dragon";
	QString teapotStr = "Teapot";

	QString particleSystemStr = "Particle System";

	QString waterTile1Str = "Simple DUDV";
	QString waterTile2Str = "Directional Flow";
	QString waterTile3Str = "Texture Distortion";
	QString waterTile4Str = "Gerstner Wave";

	QString CatmutRollStr = "Catmut Roll";
	QString NonUniformBSplineStr = "Non Uniform B Spline";
	QString NURBSStr = "NURBS";


	QPoint menuPos;
	if(!fromMainWindow) menuPos = tree->mapToGlobal(pos);
	else menuPos = tree->mapFromParent(pos);

    QMenu myMenu;

    QMenu* addObjectMenu = new QMenu("Add Object");
    addObjectMenu->addAction(cubeStr);
    addObjectMenu->addAction(sphereStr);
    addObjectMenu->addAction(circleStr);
    addObjectMenu->addAction(quadStr);
    addObjectMenu->addAction(capsuleStr);
    addObjectMenu->addAction(coneStr);
    addObjectMenu->addAction(cylinderStr);
    addObjectMenu->addAction(emptyStr);

	addObjectMenu->addSeparator();

    addObjectMenu->addAction(dirLightStr);
    addObjectMenu->addAction(pointLightStr);
    addObjectMenu->addAction(spotLightStr);

	QMenu* addMeshesMenu = new QMenu("Meshes");
    addMeshesMenu->addAction(rabbitStr);
    addMeshesMenu->addAction(buddhaStr);
    addMeshesMenu->addAction(dragonStr);
    addMeshesMenu->addAction(teapotStr);
    addObjectMenu->addMenu(addMeshesMenu);

	addObjectMenu->addSeparator();

    addObjectMenu->addAction(particleSystemStr);

	addObjectMenu->addSeparator();
    
	QMenu* addWaterTilesMenu = new QMenu("Water tiles");
    addWaterTilesMenu->addAction(waterTile1Str);
    addWaterTilesMenu->addAction(waterTile2Str);
    addWaterTilesMenu->addAction(waterTile3Str);
    addWaterTilesMenu->addAction(waterTile4Str);
    addObjectMenu->addMenu(addWaterTilesMenu);

	QMenu* addCurveMenu = new QMenu("Curves");
    addCurveMenu->addAction(CatmutRollStr);
    addCurveMenu->addAction(NonUniformBSplineStr);
    addCurveMenu->addAction(NURBSStr);
    addObjectMenu->addMenu(addCurveMenu);

	myMenu.addMenu(addObjectMenu);
    QAction* selectedItem = myMenu.exec(menuPos);


    if (selectedItem)
    {
		QModelIndexList	selectedIndexes = tree->selectionModel()->selectedIndexes();
		
		view3D->view3DGL->scene->glWindow->makeCurrent();		
		CoreEngine::Object3D* objectToAdd = nullptr;
		QString name = QString("New ") + selectedItem->text();
		if (selectedItem->text() == cubeStr) objectToAdd = CoreEngine::GetCube(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == sphereStr) objectToAdd = CoreEngine::GetSphere(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == circleStr) objectToAdd = CoreEngine::GetCircle(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == quadStr) objectToAdd = CoreEngine::GetQuad(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == capsuleStr) objectToAdd = CoreEngine::GetCapsule(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == coneStr) objectToAdd = CoreEngine::GetCone(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == cylinderStr) objectToAdd = CoreEngine::GetCylinder(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == emptyStr) objectToAdd = new CoreEngine::Object3D(name.toStdString(), view3D->view3DGL->scene);
		
		if (selectedItem->text() == dirLightStr) objectToAdd = CoreEngine::GetDirectionalLight(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(30, 0, 0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
		if (selectedItem->text() == pointLightStr) objectToAdd = CoreEngine::GetPointLight(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
		if (selectedItem->text() == spotLightStr) objectToAdd = CoreEngine::GetSpotLight(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1), 90);
		
		if (selectedItem->text() == rabbitStr) objectToAdd = CoreEngine::ObjectFromModelFile(view3D->view3DGL->scene, name.toStdString(), "resources/Models/bunny/bunny.obj");
		if (selectedItem->text() == buddhaStr) objectToAdd = CoreEngine::ObjectFromModelFile(view3D->view3DGL->scene, name.toStdString(), "resources/Models/buddha/buddha.obj");
		if (selectedItem->text() == dragonStr) objectToAdd = CoreEngine::ObjectFromModelFile(view3D->view3DGL->scene, name.toStdString(), "resources/Models/dragon/dragon.obj");
		if (selectedItem->text() == teapotStr) objectToAdd = CoreEngine::ObjectFromModelFile(view3D->view3DGL->scene, name.toStdString(), "resources/Models/teapot/teapot.obj");
		
		if (selectedItem->text() == particleSystemStr) objectToAdd = CoreEngine::GetParticleSystem(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		
		if (selectedItem->text() == waterTile1Str) objectToAdd = CoreEngine::GetWaterTile_1(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == waterTile2Str) objectToAdd = CoreEngine::GetWaterTile_2(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == waterTile3Str) objectToAdd = CoreEngine::GetWaterTile_3(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
		if (selectedItem->text() == waterTile4Str) objectToAdd = CoreEngine::GetWaterTile_4(view3D->view3DGL->scene, name.toStdString(), glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));

		if (selectedItem->text() == CatmutRollStr) objectToAdd = CoreEngine::GetCatmutRollSpline(view3D->view3DGL->scene, name.toStdString(),glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0));
		if (selectedItem->text() == NonUniformBSplineStr) objectToAdd = CoreEngine::GetNonUniformBSpline(view3D->view3DGL->scene, name.toStdString(),glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0));
		if (selectedItem->text() == NURBSStr) objectToAdd = CoreEngine::GetNURBS(view3D->view3DGL->scene, name.toStdString(),glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(0));


		// if (selectedIndexes.size() > 0) {
		// 	TreeItem* parentItem = (TreeItem*)model->itemFromIndex(selectedIndexes[0]);
		// 	parentItem->object3D->AddObject(objectToAdd);

		// 	TreeItem* itemToAdd = new TreeItem(QString(name));
		// 	itemToAdd->object3D = objectToAdd;
		// 	parentItem->appendRow(itemToAdd);
		// }
		// else {
			view3D->view3DGL->scene->AddObject(objectToAdd);
		// }

		view3D->view3DGL->scene->triggerRefresh = true;
		view3D->view3DGL->scene->glWindow->doneCurrent();		
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

void SceneTree::DeleteObject(CoreEngine::Object3D* object) {
	for (int i = 0; i < model->rowCount(); i++) {
		TreeItem* item = (TreeItem*) model->item(i, 0);
		if(item->object3D == object) {
			item->Delete();
		}
	}
}

}