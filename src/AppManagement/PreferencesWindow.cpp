#include "PreferencesWindow.hpp"

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
        layout->addWidget(hdrCheckbox);
        
        QCheckBox* gammaCorrectionCheckbox = new QCheckBox("Gamma Correction");
        layout->addWidget(gammaCorrectionCheckbox);


    }    
}