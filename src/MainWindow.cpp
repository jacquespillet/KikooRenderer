#include "MainWindow.hpp"

namespace KikooRenderer {
    MainWindow::MainWindow() {
        setWindowTitle("KikooRenderer");
        view3D= new View3D;
        setCentralWidget(view3D);
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