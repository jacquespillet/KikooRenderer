
#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
class SceneTree;

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