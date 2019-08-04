#pragma once

#include "Util/Common.h"
#include "View3D/View3D.hpp"
#include "SceneTree/SceneTree.hpp"
#include "ObjectDetails/ObjectDetails.hpp"
#include "Renderer/RayTracer.hpp"

namespace KikooRenderer 
{
class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow();
        void ExpandToFitScreen();
        View3D* view3D;
        SceneTree* sceneTree;
		 ObjectDetails* objectDetails;

        KikooRenderer::Renderer::RayTracer rayTracer;
};
}
