#pragma once

#include "Util/Common.h"
#include "View3D/View3D.hpp"
#include "SceneTree/SceneTree.hpp"
#include "ObjectDetails/ObjectDetails.hpp"
#include "Renderer/RayTracer.hpp"

#include "AppManagement/PreferencesWindow.hpp"

namespace KikooRenderer 
{
class PreferencesWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow();
        void ExpandToFitScreen();
        View3D* view3D;
        SceneTree* sceneTree;
		ObjectDetailsPanel* objectDetails;

        OfflineRenderer::RayTracer rayTracer;

        void BuildMenuBar();


		QMenu* fileMenu;
		QAction* newAction;
		QAction* saveAsAction;
		QAction* saveAction;
		QAction* loadAction;

        QMenu* optionsMenu;
		QAction* preferencesAction;
        
		QMenu* aboutMenu;
		QAction* helpAction;
		QAction* aboutAction;

		PreferencesWindow* preferencesWindow;

		std::string currentFolderPath;
};
}
