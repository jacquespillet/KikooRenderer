#include "PreferencesWindow.hpp"
#include "3DEngine/Renderers/Renderer.hpp"

namespace KikooRenderer 
{

    //////////////Window class
    PreferencesWindow::PreferencesWindow(MainWindow* mainWindow) {
        setWindowTitle("Preferences");    
        renderPreferences = new RenderPreferences(this);
        setCentralWidget(renderPreferences);
        this->mainWindow = mainWindow;
    }

    void PreferencesWindow::showEvent(QShowEvent *ev) {}
    void PreferencesWindow::closeEvent(QCloseEvent *event) {}


    /////////// Main Dock Widget class
    RenderPreferences::RenderPreferences(PreferencesWindow* mainPrefWindow) {
        setFeatures(QDockWidget::NoDockWidgetFeatures);
    
        this->mainPrefWindow = mainPrefWindow;

        baseWidget = new QWidget;
        setWidget(baseWidget); 
        layout = new QVBoxLayout;
		layout->setAlignment(Qt::AlignTop);	        
        baseWidget->setLayout(layout);

        //Background Settings
        backgroundPreferences = new BackgroundPreferences();
        layout->addWidget(backgroundPreferences);
        connect(backgroundPreferences, &BackgroundPreferences::SkyboxFileNames, this, [this]( std::vector<std::string> skyboxStrings) {
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
        });


        ui3DPreferences = new UI3DPreferences();        
        layout->addWidget(ui3DPreferences);

        QCheckBox* hdrCheckbox = new QCheckBox("HDR");
        hdrCheckbox->setChecked(true);
        connect(hdrCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state) {
            
            if(state > 0) mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetRenderPipeline(CoreEngine::RENDER_PIPELINE::HDR);
            else mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetRenderPipeline(CoreEngine::RENDER_PIPELINE::FORWARD);
        });
        layout->addWidget(hdrCheckbox);

        QCheckBox* msaaCheckbox = new QCheckBox("Multi Sampling Anti Aliasing");
        msaaCheckbox->setChecked(true);
        connect(msaaCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state) {
            mainPrefWindow->mainWindow->view3D->view3DGL->scene->glWindow->makeCurrent();
            mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderer->SetMSAA(state > 0);
            mainPrefWindow->mainWindow->view3D->view3DGL->scene->glWindow->doneCurrent();
        });
        layout->addWidget(msaaCheckbox);

        
        // QCheckBox* gammaCorrectionCheckbox = new QCheckBox("Gamma Correction");
        // connect(gammaCorrectionCheckbox, &QCheckBox::stateChanged, this, [this, mainPrefWindow](int state) {
        //     mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderer->SetGammaCorrection(state > 0);
        // });
        // layout->addWidget(gammaCorrectionCheckbox);




    }    
}