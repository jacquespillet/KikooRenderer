#include "PostProcessingPreferences.hpp"
#include "PreferencesWindow.hpp"
#include "3DEngine/PostProcessing/PostProcess.hpp"
#include "3DEngine/PostProcessing/FXAAPostProcess.hpp"
#include "3DEngine/PostProcessing/DepthOfFieldPostProcess.hpp"
#include "3DEngine/PostProcessing/bloomPostProcess.hpp"

namespace KikooRenderer 
{
PostProcessingPreferences::PostProcessingPreferences(PreferencesWindow* mainPrefWindow) {
    this->mainPrefWindow = mainPrefWindow;

    //0. FXAA Post
    // -------------------------------------------------------------------
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    QGroupBox* fxaaGroupbox = new QGroupBox("FXAA");
    QVBoxLayout* fxaaLayout = new QVBoxLayout();
    fxaaGroupbox->setLayout(fxaaLayout);

    QCheckBox* enableFxaaCheckbox = new QCheckBox("enable FXAA");
    fxaaLayout->addWidget(enableFxaaCheckbox);

    connect(enableFxaaCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state){
            scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
            scene->glWindow->makeCurrent();
        if(state > 0) {
            fxaaPost = new CoreEngine::FXAAPostProcess(scene);
            scene->renderer->AddPostEffect(fxaaPost);
        } else {
            scene->renderer->RemovePostEffect(fxaaPost);
            delete fxaaPost;
        }
        scene->glWindow->doneCurrent();
    });

    // // minValue
    // CustomSlider* minValueSlider = new CustomSlider(0.0f, 0.5, 0.0001, "minValue", (1.0 / 128.0));
    // fxaaLayout->addLayout(minValueSlider);
    // QObject::connect(minValueSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        
    //     scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
    //     scene->glWindow->makeCurrent();
        
    //     if(fxaaPost != nullptr) {
    //         fxaaPost->minValue = val;
    //     }
    //     scene->triggerRefresh = true;
    //     scene->glWindow->doneCurrent();
    // });

    // // maxSpan
    // CustomSlider* maxSpanSlider = new CustomSlider(0.0f, 20.0, 0.1, "maxSpan", 8.0f);
    // fxaaLayout->addLayout(maxSpanSlider);
    // QObject::connect(maxSpanSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        
    //     scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
    //     scene->glWindow->makeCurrent();
        
    //     if(fxaaPost != nullptr) {
    //         fxaaPost->maxSpan = val;
    //     }
    //     scene->triggerRefresh = true;
    //     scene->glWindow->doneCurrent();
    // });    

    // reduceMultiplier
    // CustomSlider* reduceMultiplierSlider = new CustomSlider(0.0f, 1, 0.001, "reduceMultiplier", (1.0 / 8.0));
    // fxaaLayout->addLayout(reduceMultiplierSlider);
    // QObject::connect(reduceMultiplierSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        
    //     scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
    //     scene->glWindow->makeCurrent();
        
    //     if(fxaaPost != nullptr) {
    //         fxaaPost->reduceMultiplier = val;
    //     }
    //     scene->triggerRefresh = true;
    //     scene->glWindow->doneCurrent();
    // });    

    // reduceMultiplier
    CustomSlider* blendFactorMultiplierSlider = new CustomSlider(0.0f, 2.0f, 0.01, "blend factor", 1.0);
    fxaaLayout->addLayout(blendFactorMultiplierSlider);
    QObject::connect(blendFactorMultiplierSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(fxaaPost != nullptr) {
            fxaaPost->blendFactorMultiplier = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });

    mainLayout->addWidget(fxaaGroupbox);
    // -------------------------------------------------------------------

    
    //1. Depth of Field Post
    // -------------------------------------------------------------------
    QGroupBox* dofGroupbox = new QGroupBox("Depth of Field");
    QVBoxLayout* dofLayout = new QVBoxLayout();
    dofGroupbox->setLayout(dofLayout);

    QCheckBox* enableDofCheckbox = new QCheckBox("enable Depth of Field");
    dofLayout->addWidget(enableDofCheckbox);

    connect(enableDofCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state){
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        if(state > 0) {
            dofPost = new CoreEngine::DepthOfFieldPostProcess(scene);
            scene->renderer->AddPostEffect(dofPost);
        } else {
            scene->renderer->RemovePostEffect(dofPost);
            delete dofPost;
        }
        scene->glWindow->doneCurrent();
    });    


    CustomSlider* bokehRadiusSlider = new CustomSlider(1.0f, 64.0f, 1.0, "Bokeh Radius", 8.0);
    dofLayout->addLayout(bokehRadiusSlider);
    QObject::connect(bokehRadiusSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(dofPost != nullptr) {
            dofPost->bokehRadius = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });
    

    CustomSlider* focusDistanceSlider = new CustomSlider(1.0f, 64.0f, 1.0, "Focus Distance", 10.0);
    dofLayout->addLayout(focusDistanceSlider);
    QObject::connect(focusDistanceSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(dofPost != nullptr) {
            dofPost->focusDistance = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });
    
    
    CustomSlider* focusRangeSlider = new CustomSlider(1.0f, 10.0f, 0.5, "Focus Range", 3.0);
    dofLayout->addLayout(focusRangeSlider);
    QObject::connect(focusRangeSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(dofPost != nullptr) {
            dofPost->focusRange = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });
    mainLayout->addWidget(dofGroupbox);
    // -------------------------------------------------------------------


    //2. Bloom Post
    // -------------------------------------------------------------------    
    QGroupBox* bloomGroupbox = new QGroupBox("Bloom");
    QVBoxLayout* bloomLayout = new QVBoxLayout();
    bloomGroupbox->setLayout(bloomLayout);

    QCheckBox* enablebloomCheckbox = new QCheckBox("enable Bloom");
    bloomLayout->addWidget(enablebloomCheckbox);

    connect(enablebloomCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state){
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        if(state > 0) {
            bloomPost = new CoreEngine::BloomPostProcess(scene);
            scene->renderer->AddPostEffect(bloomPost);
        } else {
            scene->renderer->RemovePostEffect(bloomPost);
            delete bloomPost;
        }
        scene->glWindow->doneCurrent();
    });    

    CustomSlider* brightnessThresholdSlider = new CustomSlider(0.0f, 2.0f, 0.01, "Brightness Threshold", 0.3);
    bloomLayout->addLayout(brightnessThresholdSlider);
    QObject::connect(brightnessThresholdSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(bloomPost != nullptr) {
            bloomPost->brightnessThreshold = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });

    CustomSlider* kernelSizeSlider = new CustomSlider(3.0f, 21.0f, 2.0, "Kernel Size", 5.0f);
    bloomLayout->addLayout(kernelSizeSlider);
    QObject::connect(kernelSizeSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(bloomPost != nullptr) {
            bloomPost->kernelSize = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });

    CustomSlider* kernelRadiusSlider = new CustomSlider(1.0f, 16.0f, 0.5, "Kernel Radius", 1.0f);
    bloomLayout->addLayout(kernelRadiusSlider);
    QObject::connect(kernelRadiusSlider, &CustomSlider::Modified, [this, mainPrefWindow](double val) {
        scene = mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        
        if(bloomPost != nullptr) {
            bloomPost->kernelRadius = val;
        }
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });

    mainLayout->addWidget(bloomGroupbox);
    // -------------------------------------------------------------------    

}

}