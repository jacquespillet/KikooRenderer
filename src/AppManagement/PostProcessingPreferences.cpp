#include "PostProcessingPreferences.hpp"
#include "PreferencesWindow.hpp"
#include "3DEngine/PostProcessing/PostProcess.hpp"
#include "3DEngine/PostProcessing/FXAAPostProcess.hpp"

namespace KikooRenderer 
{
PostProcessingPreferences::PostProcessingPreferences(PreferencesWindow* mainPrefWindow) {
    this->mainPrefWindow = mainPrefWindow;


    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    QGroupBox* grayScaleGroupbox = new QGroupBox("Gray Scale Post FX");
    QVBoxLayout* grayScaleLayout = new QVBoxLayout();
    grayScaleGroupbox->setLayout(grayScaleLayout);

    QCheckBox* enableGrayScaleCheckbox = new QCheckBox("Enable Grayscale post FX");
    grayScaleLayout->addWidget(enableGrayScaleCheckbox);

    connect(enableGrayScaleCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state){
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        if(state > 0) {
            // grayScalePost = new CoreEngine::PostProcess(scene);
            grayScalePost = new CoreEngine::FXAAPostProcess(scene);
            scene->renderer->AddPostEffect(grayScalePost);
        } else {
            //Remove the live effect from the scene
            scene->renderer->RemovePostEffect(grayScalePost);
            delete grayScalePost;
        }
        scene->glWindow->doneCurrent();
    });

    mainLayout->addWidget(grayScaleGroupbox);
}

}