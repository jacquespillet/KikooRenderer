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

    // minValue
    CustomSlider* minValueSlider = new CustomSlider(0.0f, 0.5, 0.0001, "minValue", (1.0 / 128.0));
    grayScaleLayout->addLayout(minValueSlider);
    QObject::connect(minValueSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(fxaaPost != nullptr) {
            fxaaPost->minValue = val;
        }
        scene->triggerRefresh = true;
    });

    // maxSpan
    CustomSlider* maxSpanSlider = new CustomSlider(0.0f, 20.0, 0.1, "maxSpan", 8.0f);
    grayScaleLayout->addLayout(maxSpanSlider);
    QObject::connect(maxSpanSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(fxaaPost != nullptr) {
            fxaaPost->maxSpan = val;
        }
        scene->triggerRefresh = true;
    });    

    // reduceMultiplier
    CustomSlider* reduceMultiplierSlider = new CustomSlider(0.0f, 1, 0.001, "reduceMultiplier", (1.0 / 8.0));
    grayScaleLayout->addLayout(reduceMultiplierSlider);
    QObject::connect(reduceMultiplierSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(fxaaPost != nullptr) {
            fxaaPost->reduceMultiplier = val;
        }
        scene->triggerRefresh = true;
    });    


    connect(enableGrayScaleCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state){
        // if(scene != nullptr) {
            scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
            scene->glWindow->makeCurrent();
        // }
        if(state > 0) {
            // grayScalePost = new CoreEngine::PostProcess(scene);
            fxaaPost = new CoreEngine::FXAAPostProcess(scene);
            scene->renderer->AddPostEffect(fxaaPost);
        } else {
            //Remove the live effect from the scene
            scene->renderer->RemovePostEffect(fxaaPost);
            delete fxaaPost;
        }
        scene->glWindow->doneCurrent();
    });

    mainLayout->addWidget(grayScaleGroupbox);
}

}