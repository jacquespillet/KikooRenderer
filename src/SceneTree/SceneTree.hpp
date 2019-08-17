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
		QWidget* layoutWindow;
        View3D* view3D;
		SceneTreeView *tree;
        QStandardItemModel* model;

		void Refresh();
		void DeleteObject(CoreEngine::Object3D* object);

	protected:
		virtual void keyPressEvent(QKeyEvent* e);
	public slots:
        void ShowContextMenu(const QPoint& pos, bool fromMainWindow=false);
		void OnItemChanged(QStandardItem* item);
};

class TreeItem : public QObject, public QStandardItem  {
    Q_OBJECT
    public:
        TreeItem(QString name) : QStandardItem(name) {
			setDragEnabled(true);
			setDropEnabled(true);
			setEditable(true);
        }
		void Refresh();
		void Delete();

		CoreEngine::Object3D* object3D;
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