#include "PreferencesWindow.hpp"

namespace KikooRenderer 
{

    //////////////Window class
    PreferencesWindow::PreferencesWindow(MainWindow* mainWindow) {
        setWindowTitle("Preferences");    
        renderPreferences = new RenderPreferences(this);
        setCentralWidget(renderPreferences);
        this->mainWindow = mainWindow;

        //Left side widget for option categories ? ...
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
        QGroupBox* backgroundGroupBox = new QGroupBox("Scene Background Settings");
        QVBoxLayout* backgroundGroupLayout = new QVBoxLayout();
        backgroundGroupBox->setLayout(backgroundGroupLayout);

        FilePicker* skyboxPicker = new FilePicker("Scene Skybox", "", true);
        backgroundGroupLayout->addWidget(skyboxPicker);
        connect(skyboxPicker, &FilePicker::FilesModified, this, [this, mainPrefWindow](QStringList filenames) {
            if(filenames.size() == 6) {
                std::vector<std::string> stdfilenames;
                for(int i=0; i<filenames.size(); i++) {
                    stdfilenames.push_back(filenames[i].toStdString());
                }
                // mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(stdfilenames);
            }
        });

        QCheckBox* solidBackgroundColorCheckBox = new QCheckBox("Solid Background Color");
        backgroundGroupLayout->addWidget(solidBackgroundColorCheckBox);
        connect(solidBackgroundColorCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        });
        
        ColorPicker* backgroundColorPicker = new ColorPicker("Background Color", 120, 120, 120, 255);
        backgroundGroupLayout->addWidget(backgroundColorPicker);
        connect(backgroundColorPicker, &ColorPicker::ColorPicked, this, [this](QColor color) {
        });

        layout->addWidget(backgroundGroupBox);


    }    
}