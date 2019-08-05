#pragma once

#include "Util/Common.h"
#include "View3D/View3D.hpp"
#include "3DEngine/BaseObjects.hpp"

namespace KikooRenderer {

class View3D;
class SceneTreeView;

class SceneTree : public QDockWidget
{
    Q_OBJECT
    public: 
        SceneTree();
        // View3DGL* view3DGL;
		QWidget* layoutWindow;
        View3D* view3D;
		SceneTreeView *tree;
        QStandardItemModel* model;


	public slots:
        void ShowContextMenu(const QPoint& pos);
};

class TreeItem : public QObject, public QStandardItem  {
    Q_OBJECT
    public:
        TreeItem(QString name) : QStandardItem(name) {
        }
		CoreEngine::Object3D* object3D;
	public slots:   
};

class SceneTreeView : public QTreeView
{
public:
    SceneTreeView() : QTreeView() {}
    virtual ~SceneTreeView() {}
	SceneTree* sceneTree;	
	QStandardItemModel* model;


private:
	virtual void mousePressEvent(QMouseEvent *event);
};


}