#pragma once 
#include "Common/Common.h"

#include <QtGui/QWindow>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDoubleSpinBox>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QExposeEvent>
#include <QtCore/QEvent>
#include <QtCore/QTimer>
#include <QtGui/QPainter>

#include "../3DEngine/Scene.hpp"

namespace KikooRenderer {
    
class View3DGL: public QOpenGLWidget {
    Q_OBJECT
    public: 
        View3DGL();
        ~View3DGL();

		CoreEngine::Scene* scene; 

    protected: 
        int windowWidth;
        int windowHeight;
        QTimer* timer;
        
        //Overriden functions for GL
        std::string GetGLErr(); 
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();


        //Event Listeners
        virtual void keyPressEvent(QKeyEvent *e);
        virtual void keyReleaseEvent(QKeyEvent *e);
        virtual void mousePressEvent(QKeyEvent *e);
        virtual void mouseReleaseEvent(QKeyEvent *e);
        virtual void mouseMoveEvent(QKeyEvent *e);
        virtual void wheelEvent(QKeyEvent *e);

    //public slots: ??  
        void Refresh();
        void OnCloseEvent();
};

}