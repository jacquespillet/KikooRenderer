#pragma once

#include "Util/Common.h"
#include "View3D/View3D.hpp"

namespace KikooRenderer {
namespace CoreEngine{
	class Object3D;
}
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
		void AddObject(CoreEngine::Object3D* object, CoreEngine::Object3D* parent=nullptr);

	protected:
		virtual void keyPressEvent(QKeyEvent* e);
	public slots:
        void ShowContextMenu(const QPoint& pos, bool fromMainWindow=false);
		void OnItemChanged(QStandardItem* item);
};



}