#include "View3DGL.hpp"


#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
    //Constructor & Destructor
    View3DGL::View3DGL() {
        QSurfaceFormat format;
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setSwapInterval(1);
        format.setSamples(8);
        setFormat(format);

        setFocusPolicy(Qt::ClickFocus);

        timer = new QTimer(this);
	    scene = new CoreEngine::Scene; 
        
        setMouseTracking(true);
                
    }

    View3DGL::~View3DGL() {

    }


    //GL Overriden functions
    std::string View3DGL::GetGLErr()
    {
        GETGL
        std::string err = "";
        GLenum glError = ogl->glGetError();
        if(glError == GL_INVALID_ENUM) err = "GL_INVALID_ENUM";
        else if(glError == GL_INVALID_VALUE) err = "GL_INVALID_VALUE";
        else if(glError == GL_INVALID_OPERATION) err = "GL_INVALID_OPERATION";
        else if(glError == GL_STACK_OVERFLOW) err = "GL_STACK_OVERFLOW";
        else if(glError == GL_STACK_UNDERFLOW) err = "GL_STACK_UNDERFLOW";
        else if(glError == GL_OUT_OF_MEMORY) err = "GL_OUT_OF_MEMORY";
        else if(glError == GL_INVALID_FRAMEBUFFER_OPERATION) err = "GL_INVALID_FRAMEBUFFER_OPERATION";
        else if(glError == GL_TABLE_TOO_LARGE) err = "GL_TABLE_TOO_LARGE";
        else err = "NO ERROR";
        return err;
    }


    void View3DGL::initializeGL() {
        if(!scene->started) {
            scene->Start();
        }
        scene->Enable();

        connect(timer, &QTimer::timeout, [this]() {
            makeCurrent();
            // scene->Update();

            // if(scene->triggerRefresh) {
            //     Refresh();
            //     scene->triggerRefresh=false;
            // }

            doneCurrent();
        });

        timer->start(10);

        connect(context(), &QOpenGLContext::aboutToBeDestroyed, [this]() {
            makeCurrent();
            timer->stop();
            // scene->Disable();
            doneCurrent();
        });

        Refresh();
    }

    void View3DGL::resizeGL(int w, int h) {
        windowWidth = w;
        windowHeight = h;

        scene->SetWindowSize(w, h);
        
        Refresh();
    }

    void View3DGL::paintGL() {
        scene->Render();

        std::string errstr = GetGLErr();
        if(errstr != "NO ERROR") std::cerr << "View3DGL:GL ERROR " << errstr << std::endl;
    }



    void View3DGL::Refresh() {
        repaint();
        update();
    }

    //Event Listeners
    void View3DGL::keyPressEvent(QKeyEvent *e) {

    }

    void View3DGL::keyReleaseEvent(QKeyEvent *e) {

    }

    void View3DGL::mousePressEvent(QKeyEvent *e) {

    }

    void View3DGL::mouseReleaseEvent(QKeyEvent *e) {

    }

    void View3DGL::mouseMoveEvent(QKeyEvent *e) {

    }

    void View3DGL::wheelEvent(QKeyEvent *e) {

    }   
}