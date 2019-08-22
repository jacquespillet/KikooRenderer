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
        



    bool RenderPreferences::IsAllPlanes() {
        bool res = true;
        for(int i=0; i<skyboxStrings.size(); i++) {
            if(skyboxStrings[i] == "") { 
                res = false;
                std::cout << i << "Is not set "<<std::endl;
                break;
            }
        }
        return res;
    }

    /////////// Main Dock Widget class
    RenderPreferences::RenderPreferences(PreferencesWindow* mainPrefWindow) {
        setFeatures(QDockWidget::NoDockWidgetFeatures);
    
        this->mainPrefWindow = mainPrefWindow;

        skyboxStrings = std::vector<std::string>(6, "");

        baseWidget = new QWidget;
        setWidget(baseWidget); 
        layout = new QVBoxLayout;
		layout->setAlignment(Qt::AlignTop);	        
        baseWidget->setLayout(layout);

        //Background Settings
        QGroupBox* backgroundGroupBox = new QGroupBox("Scene Background Settings");
        QVBoxLayout* backgroundGroupLayout = new QVBoxLayout();
        backgroundGroupBox->setLayout(backgroundGroupLayout);
        
        //- Right - Left - Top - Bottom - Front - Back

        FilePicker* skyboxRightPicker = new FilePicker("Right Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxRightPicker);
        connect(skyboxRightPicker, &FilePicker::FileModified, this, [this, mainPrefWindow](QString filenames) {
            std::cout << "RightPicker"<<std::endl;
            if(filenames.size() > 0) {
                skyboxStrings[0] = filenames.toStdString();
            }
            if(IsAllPlanes()) {
                mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
                std::cout << "Commiting"<<std::endl;
            }            
        });


        FilePicker* skyboxLeftPicker = new FilePicker("Left Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxLeftPicker);
        connect(skyboxLeftPicker, &FilePicker::FileModified, this, [this, mainPrefWindow](QString filenames) {
            std::cout << "LeftPicker"<<std::endl;
            if(filenames.size() > 0) {
                skyboxStrings[1] = filenames.toStdString();
            }
            if(IsAllPlanes()) {
                mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
                std::cout << "Commiting"<<std::endl;
            }                
        });

        
        FilePicker* skyboxTopPicker = new FilePicker("Top Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxTopPicker);
        connect(skyboxTopPicker, &FilePicker::FileModified, this, [this, mainPrefWindow](QString filenames) {
            std::cout << "TopPicker"<<std::endl;
            if(filenames.size() > 0) {
                skyboxStrings[2] = filenames.toStdString();
            }
            if(IsAllPlanes()) {
                mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
                std::cout << "Commiting"<<std::endl;
            }                
        });

        
        FilePicker* skyboxBottomPicker = new FilePicker("Bottom Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxBottomPicker);
        connect(skyboxBottomPicker, &FilePicker::FileModified, this, [this, mainPrefWindow](QString filenames) {
            std::cout << "BottomPicker"<<std::endl;
            if(filenames.size() > 0) {
                skyboxStrings[3] = filenames.toStdString();
            }
            if(IsAllPlanes()) {
                mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
                std::cout << "Commiting"<<std::endl;
            }                
        });

        
        FilePicker* skyboxFrontPicker = new FilePicker("Front Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxFrontPicker);
        connect(skyboxFrontPicker, &FilePicker::FileModified, this, [this, mainPrefWindow](QString filenames) {
            std::cout << "FrontPicker"<<std::endl;
            if(filenames.size() > 0) {
                skyboxStrings[4] = filenames.toStdString();
            }
            if(IsAllPlanes()) {
                mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
                std::cout << "Commiting"<<std::endl;
            }                
        });

        
        FilePicker* skyboxBackPicker = new FilePicker("Back Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxBackPicker);
        connect(skyboxBackPicker, &FilePicker::FileModified, this, [this, mainPrefWindow](QString filenames) {
            std::cout << "BackPicker"<<std::endl;
            if(filenames.size() > 0) {
                skyboxStrings[5] = filenames.toStdString();
            }
            if(IsAllPlanes()) {
                mainPrefWindow->mainWindow->view3D->view3DGL->scene->SetSkybox(skyboxStrings);
                std::cout << "Commiting"<<std::endl;
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