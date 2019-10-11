#include "View3D.hpp"
#include "../3DEngine/TransformWidget.hpp"


namespace KikooRenderer 
{

View3D::View3D() : QDockWidget("3D View")
{
    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    root = new View3DRoot;
    QVBoxLayout* layout = new QVBoxLayout;


    view3DGL = new View3DGL;

    layout->addWidget(view3DGL);

    
    root->setLayout(layout);
    setWidget(root);

	setFocusPolicy(Qt::StrongFocus);

    QToolBar *viewportToolbar = new QToolBar(tr("File"));
    viewportToolbar->setStyleSheet("QToolBar{spacing:-10px;}");


    QIcon translateIcon("resources/UI/Widgets/TranslationIcon.png");
    QAction *translateAct = new QAction(translateIcon, tr("&Translate"), this);
    translateAct->setCheckable(true);
    viewportToolbar->addAction(translateAct);
    
    QIcon scaleIcon("resources/UI/Widgets/ScaleIcon.png");
    QAction *scaleAct = new QAction(scaleIcon, tr("&Scale"), this);
    scaleAct->setCheckable(true);
    viewportToolbar->addAction(scaleAct);
    
    QIcon rotationIcon("resources/UI/Widgets/RotationIcon.png");
    QAction *rotationAct = new QAction(rotationIcon, tr("&Rotate"), this);
    rotationAct->setCheckable(true);
    viewportToolbar->addAction(rotationAct);

    connect(translateAct, &QAction::changed, this, [this, scaleAct, rotationAct](){
        view3DGL->scene->transformWidget->SetTransformMode(CoreEngine::TransformWidget::TransformMode::TRANSLATE);
        view3DGL->scene->triggerRefresh = true;

    });

    connect(scaleAct, &QAction::changed, this, [this, rotationAct, translateAct](){
        view3DGL->scene->transformWidget->SetTransformMode(CoreEngine::TransformWidget::TransformMode::SCALE);
        view3DGL->scene->triggerRefresh = true;
    });

    
    connect(rotationAct, &QAction::changed, this, [this, scaleAct, translateAct](){
        view3DGL->scene->transformWidget->SetTransformMode(CoreEngine::TransformWidget::TransformMode::ROTATE);
        view3DGL->scene->triggerRefresh = true;
    });

    viewportToolbar->addSeparator();

 viewportToolbar->addSeparator();


    QIcon frontIcon("resources/UI/Widgets/frontView.png");
    QAction *frontAct = new QAction(frontIcon, tr("&front"), this);
	frontAct->setCheckable(true);
	viewportToolbar->addAction(frontAct);
	connect(frontAct,  &QAction::toggled, this, [this, frontAct](){
	
    });



    QIcon backIcon("resources/UI/Widgets/backView.png");
    QAction *backAct = new QAction(backIcon, tr("&back"), this);
	backAct->setCheckable(true);
	viewportToolbar->addAction(backAct);
	connect(backAct, &QAction::toggled, this, [this, backAct](){
	
    });

	
    QIcon leftIcon("resources/UI/Widgets/leftView.png");
    QAction *leftAct = new QAction(leftIcon, tr("&left"), this);
	leftAct->setCheckable(true);
	viewportToolbar->addAction(leftAct);
	connect(leftAct, &QAction::toggled, this, [this, leftAct](){
	
    });
	
    QIcon rightIcon("resources/UI/Widgets/rightView.png");
    QAction *rightAct = new QAction(rightIcon, tr("&right"), this);
	rightAct->setCheckable(true);
	viewportToolbar->addAction(rightAct);
	connect(rightAct, &QAction::toggled, this, [this, rightAct](){
        
    });
	
    QIcon topIcon("resources/UI/Widgets/topView.png");
    QAction *topAct = new QAction(topIcon, tr("&top"), this);
	topAct->setCheckable(true);
	viewportToolbar->addAction(topAct);
	connect(topAct, &QAction::toggled, this, [this, topAct](){
	
    });

	
    QIcon bottomIcon("resources/UI/Widgets/bottomView.png");
    QAction *bottomAct = new QAction(bottomIcon, tr("&bottom"), this);
	bottomAct->setCheckable(true);
	viewportToolbar->addAction(bottomAct);
	connect(bottomAct, &QAction::toggled, this, [this, bottomAct](){
    });

    viewportToolbar->addSeparator();

	
    QIcon fullScreenIcon("resources/UI/Widgets/FullScreen.png");
    QAction *fullScreenAct = new QAction(fullScreenIcon, tr("&fullScreen"), this);
	fullScreenAct->setCheckable(true);
	viewportToolbar->addAction(fullScreenAct);
	connect(fullScreenAct, &QAction::toggled, this, [this, fullScreenAct](){
        std::cout << "HERE" << std::endl;

        // View3DGL* fs = new View3DGL(view3DGL->scene); 
        // // fs->scene = view3DGL->scene;        
        // sceneWindow = new QMainWindow();
        // sceneWindow->setCentralWidget(fs);
        // sceneWindow->show();

    });


    QIcon snapShotIcon("resources/UI/Widgets/snapShot.png");
    QAction *snapShotAct = new QAction(snapShotIcon, tr("&snapShot"), this);
	snapShotAct->setCheckable(true);
	viewportToolbar->addAction(snapShotAct);
	connect(snapShotAct, &QAction::toggled, this, [this, snapShotAct](){
        QImage img = view3DGL->grabFramebuffer();
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("(*.png)"));
        if(fileName.length() > 0)
        {
            img.save(fileName);
        }        
    });


    layout->addWidget(viewportToolbar);    
        
}
void View3D::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Space) {
		QPoint pos = QCursor::pos();
		sceneTree->ShowContextMenu(pos, true);
	}

	view3DGL->OnkeyPressEvent(e);
}
}	