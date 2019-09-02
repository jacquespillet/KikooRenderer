#include "View3DGL.hpp"
#include <ctime>

#include "3DEngine/BaseObjects.hpp"


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
        scene->glWindow = this;
        
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
            GETGL
            //
            //Set GL states
            //
            ogl->glEnable(GL_CULL_FACE);  
            ogl->glCullFace(GL_BACK);
            
            ogl->glEnable(GL_BLEND);
            ogl->glEnable(GL_STENCIL_TEST);    
            ogl->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
            ogl->glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should update the stencil buffer
            ogl->glStencilMask(0xFF); // enable writing to the stencil buffer
            
            ogl->glEnable(GL_MULTISAMPLE);  
            ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            //disable writting to depth buffer
            ogl->glEnable(GL_DEPTH_TEST);
            //glDepthMask(GL_FALSE);
            // ogl->glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			

            //enable blending
            //glBlendFunc(GL_ZERO, GL_SRC_COLOR);
            //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE);            

			scene->Start();
        }
        scene->Enable();

        connect(timer, &QTimer::timeout, [this]() {
            std::time_t result = std::time(nullptr);
            
            clock_t currentTime = clock();
            scene->deltaTime =  double( clock () - scene->previousTime ) /  CLOCKS_PER_SEC;
            scene->previousTime = currentTime; 

            makeCurrent();
            scene->OnUpdate();  

            if(scene->triggerRefresh) {
                Refresh();
                scene->triggerRefresh=false;
            }

            doneCurrent();
        });

        timer->start(10);

        connect(context(), &QOpenGLContext::aboutToBeDestroyed, [this]() {
            makeCurrent();
            timer->stop();
            scene->OnDestroy();
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
    void View3DGL::OnkeyPressEvent(QKeyEvent *e) {
        scene->OnKeyPressEvent(e);
        Refresh();
    }

    void View3DGL::keyReleaseEvent(QKeyEvent *e) {
        scene->OnKeyReleaseEvent(e);
		Refresh();
	}

    void View3DGL::mousePressEvent(QMouseEvent *e) {
        scene->OnMousePressEvent(e);
        Refresh();
    }

    void View3DGL::mouseReleaseEvent(QMouseEvent *e) {
        scene->OnMouseReleaseEvent(e);
        Refresh();
    }

    void View3DGL::mouseMoveEvent(QMouseEvent *e) {
        scene->OnMouseMoveEvent(e);
        Refresh();
    }

    void View3DGL::wheelEvent(QWheelEvent *e) {
        scene->OnWheelEvent(e);
        Refresh();
    }   
}