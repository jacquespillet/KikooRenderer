#include "MainWindow.hpp"

#include "FileIO/SceneToFile.hpp"

namespace KikooRenderer {
    MainWindow::MainWindow() {
        setWindowTitle("KikooRenderer");

        preferencesWindow = new PreferencesWindow(this);
        
	    BuildMenuBar();

        view3D= new View3D;
        sceneTree= new SceneTree;
		objectDetails = new ObjectDetailsPanel;
		consoleDock = new ConsoleDock;

		view3D->view3DGL->scene->objectDetailsPanel = objectDetails;
		objectDetails->sceneTree = sceneTree;
        sceneTree->view3D = view3D;
        view3D->sceneTree = sceneTree;
		view3D->view3DGL->scene->sceneTree = view3D->sceneTree;


        //Run raytracer scene
         //rayTracer.WriteImage();

        setCentralWidget(view3D);
        addDockWidget(Qt::LeftDockWidgetArea, sceneTree);
        addDockWidget(Qt::RightDockWidgetArea, objectDetails);
        addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

        setDockOptions(QMainWindow::AnimatedDocks);

        show();
	    // showMaximized();
        // ExpandToFitScreen();
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

    void MainWindow::BuildMenuBar()
    {
        fileMenu = menuBar()->addMenu("File");
        newAction = fileMenu->addAction("New Scene");
        saveAction = fileMenu->addAction("Save Scene");
        saveAsAction = fileMenu->addAction("Save Scene As");
        loadAction = fileMenu->addAction("Load Scene");

        optionsMenu = menuBar()->addMenu("Options");
        preferencesAction = optionsMenu->addAction("Preferences");

        aboutMenu =  menuBar()->addMenu("About");
        helpAction = aboutMenu->addAction("Help");
        aboutAction = aboutMenu->addAction("About");
        
        // QObject::connect(newAction, SIGNAL(triggered()), this, SLOT(NewProject()));
        
        QObject::connect(saveAction, &QAction::triggered, this, [this]() {
            QString fileName = QFileDialog::getSaveFileName(this, tr("Save Scene"), tr(currentFolderPath.c_str()), tr("Scene File (*.kikooScene)"));
            if(fileName.length() > 0)
            {
                std::string currentProjectFilePath = fileName.toStdString();
                currentFolderPath = QFileInfo(fileName).absoluteDir().absolutePath().toStdString();
                SceneToFile(view3D->view3DGL->scene, fileName.toStdString());
            }
        });
        
        // QObject::connect(saveAsAction, SIGNAL(triggered()), this, SLOT(SaveAsProject()));

        QObject::connect(loadAction, &QAction::triggered, this, [this]() {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open Scene"), tr(currentFolderPath.c_str()), tr("Scene File (*.kikooScene)"));
            if(fileName.length() > 0)
            {
                SceneFromFile(view3D->view3DGL->scene, fileName.toStdString());
                setWindowTitle(QFileInfo(fileName).fileName() + " - KikooRenderer");
            }            
        });
        
        //QObject::connect(view3DAction, SIGNAL(triggered()), this, SLOT(Start3DView()));
        QObject::connect(preferencesAction, &QAction::triggered, this, [this]() {
            preferencesWindow->show();
        });
    }    
}