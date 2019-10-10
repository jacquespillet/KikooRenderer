#pragma once

#include "Util/Common.h"

namespace KikooRenderer {

namespace CoreEngine{
	class Object3D;
}


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

}