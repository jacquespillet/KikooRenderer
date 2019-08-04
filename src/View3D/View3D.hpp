#pragma once

#include "Util/Common.h"
#include "SceneTree/SceneTree.hpp"
#include "View3DGL.hpp"

namespace KikooRenderer {

class View3DRoot: public QWidget
{
    Q_OBJECT
    public:
        View3DRoot() {
            setMinimumWidth(250);
        }

        QSize sizeHint() const {
            return QSize(500, 0);
        }

    protected:
        void resizeEvent(QResizeEvent* event) {}
};

class SceneTree;
class View3D : public QDockWidget
{
    Q_OBJECT
    public: 
        View3D();
        SceneTree* sceneTree;

        View3DGL* view3DGL;
        View3DRoot* root;
		QWidget* layoutWindow;

};

}