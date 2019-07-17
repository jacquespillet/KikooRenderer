#pragma once

#include "Common/Common.h"
#include "View3D/View3D.hpp"
#include "SceneTree/SceneTree.hpp"
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

        KikooRenderer::Renderer::RayTracer rayTracer;
};
}
