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
    translateAct->setChecked(true);
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
        scaleAct->setChecked(false);
        rotationAct->setChecked(false);
        view3DGL->scene->transformWidget->SetTransformMode(CoreEngine::TransformWidget::TransformMode::TRANSLATE);
        view3DGL->scene->triggerRefresh = true;

    });

    connect(scaleAct, &QAction::changed, this, [this, rotationAct, translateAct](){
        translateAct->setChecked(false);
        rotationAct->setChecked(false);
        view3DGL->scene->transformWidget->SetTransformMode(CoreEngine::TransformWidget::TransformMode::SCALE);
        view3DGL->scene->triggerRefresh = true;
    });

    
    connect(rotationAct, &QAction::changed, this, [this, scaleAct, translateAct](){
        scaleAct->setChecked(false);
        translateAct->setChecked(false);
        view3DGL->scene->transformWidget->SetTransformMode(CoreEngine::TransformWidget::TransformMode::ROTATE);
        view3DGL->scene->triggerRefresh = true;
    });
    
    viewportToolbar->addSeparator();

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