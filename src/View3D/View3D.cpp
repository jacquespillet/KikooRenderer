#include "View3D.hpp"
#include "../3DEngine/TransformWidget.hpp"
#include "../3DEngine/CameraControllers/CameraController.hpp"
#include "Util/QtUtil.hpp"

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
        view3DGL->scene->camera->transform->StartAnimate(glm::vec3(0, 0, -5), glm::vec3(0), glm::vec3(1));
    });



    QIcon backIcon("resources/UI/Widgets/backView.png");
    QAction *backAct = new QAction(backIcon, tr("&back"), this);
	backAct->setCheckable(true);
	viewportToolbar->addAction(backAct);
	connect(backAct, &QAction::toggled, this, [this, backAct](){
        view3DGL->scene->camera->transform->StartAnimate(glm::vec3(0, 0, 5), glm::vec3(0, 180, 0), glm::vec3(1));
    });

	
    QIcon leftIcon("resources/UI/Widgets/leftView.png");
    QAction *leftAct = new QAction(leftIcon, tr("&left"), this);
	leftAct->setCheckable(true);
	viewportToolbar->addAction(leftAct);
	connect(leftAct, &QAction::toggled, this, [this, leftAct](){
	    view3DGL->scene->camera->transform->StartAnimate(glm::vec3(-5, 0, 0), glm::vec3(0, 90, 0), glm::vec3(1));
    });
	
    QIcon rightIcon("resources/UI/Widgets/rightView.png");
    QAction *rightAct = new QAction(rightIcon, tr("&right"), this);
	rightAct->setCheckable(true);
	viewportToolbar->addAction(rightAct);
	connect(rightAct, &QAction::toggled, this, [this, rightAct](){
        view3DGL->scene->camera->transform->StartAnimate(glm::vec3(5, 0, 0), glm::vec3(0,-90, 0), glm::vec3(1));
    });
	
    QIcon topIcon("resources/UI/Widgets/topView.png");
    QAction *topAct = new QAction(topIcon, tr("&top"), this);
	topAct->setCheckable(true);
	viewportToolbar->addAction(topAct);
	connect(topAct, &QAction::toggled, this, [this, topAct](){
        view3DGL->scene->camera->transform->StartAnimate(glm::vec3(0, 5, 0), glm::vec3(90, 0, 0), glm::vec3(1));
    });

	
    QIcon bottomIcon("resources/UI/Widgets/bottomView.png");
    QAction *bottomAct = new QAction(bottomIcon, tr("&bottom"), this);
	bottomAct->setCheckable(true);
	viewportToolbar->addAction(bottomAct);
	connect(bottomAct, &QAction::toggled, this, [this, bottomAct](){
        view3DGL->scene->camera->transform->StartAnimate(glm::vec3(0, -5, 0), glm::vec3(-90, 0, 0), glm::vec3(1));
    });

    viewportToolbar->addSeparator();

	
    QIcon fullScreenIcon("resources/UI/Widgets/FullScreen.png");
    QAction *fullScreenAct = new QAction(fullScreenIcon, tr("&fullScreen"), this);
	fullScreenAct->setCheckable(true);
	viewportToolbar->addAction(fullScreenAct);
	connect(fullScreenAct, &QAction::toggled, this, [this, fullScreenAct](){
        //TODO
            //Duplicate scene with references to objects
    });

    layout->addWidget(viewportToolbar);   

    viewportToolbar->addSeparator();

    QIcon snapShotIcon("resources/UI/Widgets/snapshot.png");
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

    viewportToolbar->addSeparator();

    QIcon focusIcon("resources/UI/Widgets/focus.png");
    QAction *focusAct = new QAction(focusIcon, tr("&focus"), this);
	focusAct->setCheckable(true);
	viewportToolbar->addAction(focusAct);
	connect(focusAct, &QAction::toggled, this, [this](){
        view3DGL->scene->camera->GetCameraController()->MoveToFocus();
    });

    LayerController* layerController = new LayerController(view3DGL->scene->GetLayerMask());
    viewportToolbar->addWidget(layerController);
    connect(layerController, &LayerController::maskChanged, this, [this](uint16_t layerMask) {
        view3DGL->scene->SetLayerMask(layerMask);
        view3DGL->scene->triggerRefresh= true;
    });

    viewportToolbar->addSeparator();

    QIcon ButtonIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    QPushButton* playButton = new QPushButton();
    playButton->setIcon(ButtonIcon);
    viewportToolbar->addWidget(playButton);

	connect(playButton, &QPushButton::clicked, this, [this, playButton]() {
		view3DGL->scene->PlayPause();
		if(view3DGL->scene->IsPlaying()) {
			QIcon ButtonIcon(style()->standardIcon(QStyle::SP_MediaPause));
			playButton->setIcon(ButtonIcon);
		} else {
			QIcon ButtonIcon(style()->standardIcon(QStyle::SP_MediaPlay));
			playButton->setIcon(ButtonIcon);
		}
	});

    QIcon stopButtonIcon(style()->standardIcon(QStyle::SP_MediaStop));
    QPushButton* stopButton = new QPushButton();
    stopButton->setIcon(stopButtonIcon);
    viewportToolbar->addWidget(stopButton);

	connect(stopButton, &QPushButton::clicked, this, [this, stopButton]() {
		view3DGL->scene->Stop();
		if(view3DGL->scene->IsPlaying()) {
			QIcon ButtonIcon(style()->standardIcon(QStyle::SP_MediaPause));
			stopButton->setIcon(ButtonIcon);
		}
	});    


    void SetLayerMask(uint16_t _layerMask);
    uint16_t GetLayerMask();	
        
}
void View3D::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Space) {
		QPoint pos = QCursor::pos();
		sceneTree->ShowContextMenu(pos, true);
	}

	view3DGL->OnkeyPressEvent(e);
}
}	