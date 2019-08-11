#include "MainWindow.hpp"

namespace KikooRenderer {
    MainWindow::MainWindow() {
        setWindowTitle("KikooRenderer");
        view3D= new View3D;
        sceneTree= new SceneTree;
		objectDetails = new ObjectDetailsPanel;

		view3D->view3DGL->scene->objectDetailsPanel = objectDetails;
		objectDetails->sceneTree = sceneTree;
        sceneTree->view3D = view3D;
        view3D->sceneTree = sceneTree;

        //Run raytracer scene
         //rayTracer.WriteImage();

        setCentralWidget(view3D);
        addDockWidget(Qt::LeftDockWidgetArea, sceneTree);
        addDockWidget(Qt::RightDockWidgetArea, objectDetails);

        setDockOptions(QMainWindow::AnimatedDocks);

	    showNormal();
        ExpandToFitScreen();
    }

    void MainWindow::ExpandToFitScreen() {
    	QRect rec = QApplication::desktop()->availableGeometry(this);
        int differenceH = frameGeometry().height() - geometry().height();
        int differenceW = frameGeometry().width() - geometry().width();
        int height = rec.height() - differenceH;
        int width = rec.width() - differenceW;
        resize(width, height);

        move(rec.topLeft().x(), rec.topLeft().y());
        
    }
}