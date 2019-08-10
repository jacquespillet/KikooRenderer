#include "View3D.hpp"


namespace KikooRenderer 
{

View3D::View3D() : QDockWidget("3D View")
{
    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    root = new View3DRoot;
    QVBoxLayout* layout = new QVBoxLayout;

    view3DGL = new View3DGL;
	view3DGL->scene->objectDetailsPanel = objectDetailsPanel;

    layout->addWidget(view3DGL);

    
    root->setLayout(layout);
    setWidget(root);

	setFocusPolicy(Qt::StrongFocus);
	
        
}
void View3D::keyPressEvent(QKeyEvent* e) {
	if (e->key() == Qt::Key_Space) {
		QPoint pos = QCursor::pos();
		sceneTree->ShowContextMenu(pos, true);
	}

	view3DGL->OnkeyPressEvent(e);
}
}	