#pragma once

#include "Common/Common.h"

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

class View3D : public QDockWidget
{
    Q_OBJECT
    public: 
        View3D();
        View3DGL* view3DGL;
        View3DRoot* root;
		QWidget* layoutWindow;

};

}