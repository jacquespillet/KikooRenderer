#include "SceneTree.hpp"

#include "3DEngine/BaseObjects.hpp"
// #include "3DEngine/BaseObjects.hpp"

namespace KikooRenderer 
{
SceneTree::SceneTree() : QDockWidget("Scene")
{
    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    
    tree = new SceneTreeView();
    tree->setObjectName("DirView");
    tree->setUniformRowHeights(true);
    tree->setHeaderHidden(true);
    
    model = new QStandardItemModel(0, 1);
    
    tree->setModel( model ); 
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(ShowContextMenu(const QPoint&)));

    setWidget(tree);
}

void SceneTree::ShowContextMenu(const QPoint& pos)
{
    //For adding nested objects
    // std::cout << tree->currentIndex().row() << std::endl;
    // std::cout << tree->currentIndex().isValid() << std::endl;
        //             QStandardItem *child = new QStandardItem( QString("Item %0").arg(i) );
        //             child->setEditable( false );
        //             item->appendRow( child );

    QString cubeStr = "Cube";
    QString sphereStr = "Sphere";
    QString circleStr = "Circle";
    QString quadStr = "Quad";

    QPoint globalPos = tree->mapToGlobal(pos);

    QMenu myMenu;

    QMenu* addObjectMenu = new QMenu("Add Object");
    addObjectMenu->addAction(cubeStr);
    addObjectMenu->addAction(sphereStr);
    addObjectMenu->addAction(circleStr);
    addObjectMenu->addAction(quadStr);
    myMenu.addMenu(addObjectMenu);
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem)
    {
        if(selectedItem->text() == cubeStr) {
            QString name = QString("New ") + cubeStr;
            CoreEngine::Object3D* cube = CoreEngine::GetCube(view3D->view3DGL->scene, name.toStdString(),  glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));   
            std::string finalName = view3D->view3DGL->scene->AddObject(cube);
            name = QString::fromStdString(finalName);
            TreeItem *item = new TreeItem( name );
            model->appendRow(item);
        } else if(selectedItem->text() == sphereStr) {
            QString name = QString("New ") + sphereStr;
            CoreEngine::Object3D* Sphere = CoreEngine::GetSphere(view3D->view3DGL->scene, name.toStdString(),  glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));   
            std::string finalName = view3D->view3DGL->scene->AddObject(Sphere);
            name = QString::fromStdString(finalName);
            TreeItem *item = new TreeItem(name);
            model->appendRow(item);
        } else if(selectedItem->text() == circleStr) {
            QString name = QString("New ") + circleStr;
            CoreEngine::Object3D* circle = CoreEngine::GetCircle(view3D->view3DGL->scene, name.toStdString(),  glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));   
            std::string finalName = view3D->view3DGL->scene->AddObject(circle);
            name = QString::fromStdString(finalName);
            TreeItem *item = new TreeItem(name);
            model->appendRow(item);
        } else if(selectedItem->text() == quadStr) {
            QString name = QString("New ") + quadStr;
            CoreEngine::Object3D* cone = CoreEngine::GetQuad(view3D->view3DGL->scene, name.toStdString(),  glm::dvec3(0), glm::dvec3(0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));   
            std::string finalName = view3D->view3DGL->scene->AddObject(cone);
            name = QString::fromStdString(finalName);
            TreeItem *item = new TreeItem(name);
            model->appendRow(item);
        }
    }
}



}