#include "BackgroundPreferences.hpp"

namespace KikooRenderer {
    BackgroundPreferences::BackgroundPreferences(): QGroupBox("Scene Background Preferences") {
        skyboxStrings = std::vector<std::string>(6, "");

        QVBoxLayout* backgroundGroupLayout = new QVBoxLayout();
        setLayout(backgroundGroupLayout);
        
        //- Right - Left - Top - Bottom - Front - Back
        skyboxRightPicker = new FilePicker("Right Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxRightPicker);
        connect(skyboxRightPicker, &FilePicker::FileModified, this, [this](QString filenames) {
            if(filenames.size() > 0) {
                skyboxStrings[0] = filenames.toStdString();
                LookForSkybox(filenames); // Look for all the skybox files in the folder
            }
            if(IsAllPlanes()) {
                emit SkyboxFileNames(skyboxStrings);
            }
        });


        skyboxLeftPicker = new FilePicker("Left Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxLeftPicker);
        connect(skyboxLeftPicker, &FilePicker::FileModified, this, [this](QString filenames) {
            if(filenames.size() > 0) {
                skyboxStrings[1] = filenames.toStdString();
                LookForSkybox(filenames); // Look for all the skybox files in the folder
            }
            if(IsAllPlanes()) {
                emit SkyboxFileNames(skyboxStrings);
            }                
        });

        
        skyboxTopPicker = new FilePicker("Top Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxTopPicker);
        connect(skyboxTopPicker, &FilePicker::FileModified, this, [this](QString filenames) {
            if(filenames.size() > 0) {
                skyboxStrings[2] = filenames.toStdString();
                LookForSkybox(filenames); // Look for all the skybox files in the folder
            }
            if(IsAllPlanes()) {
                emit SkyboxFileNames(skyboxStrings);
            }                
        });

        
        skyboxBottomPicker = new FilePicker("Bottom Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxBottomPicker);
        connect(skyboxBottomPicker, &FilePicker::FileModified, this, [this](QString filenames) {
            if(filenames.size() > 0) {
                skyboxStrings[3] = filenames.toStdString();
                LookForSkybox(filenames); // Look for all the skybox files in the folder
            }
            if(IsAllPlanes()) {
                emit SkyboxFileNames(skyboxStrings);
            }                
        });

        
        skyboxFrontPicker = new FilePicker("Front Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxFrontPicker);
        connect(skyboxFrontPicker, &FilePicker::FileModified, this, [this](QString filenames) {
            if(filenames.size() > 0) {
                skyboxStrings[4] = filenames.toStdString();
                LookForSkybox(filenames); // Look for all the skybox files in the folder
            }
            if(IsAllPlanes()) {
                emit SkyboxFileNames(skyboxStrings);
            }                
        });

        
        skyboxBackPicker = new FilePicker("Back Skybox Plane", "", true);
        backgroundGroupLayout->addWidget(skyboxBackPicker);
        connect(skyboxBackPicker, &FilePicker::FileModified, this, [this](QString filenames) {
            if(filenames.size() > 0) {
                skyboxStrings[5] = filenames.toStdString();
                LookForSkybox(filenames); // Look for all the skybox files in the folder
            }
            if(IsAllPlanes()) {
                emit SkyboxFileNames(skyboxStrings);
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
    }


    bool BackgroundPreferences::IsAllPlanes() {
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

    void BackgroundPreferences::LookForSkybox(QString filename) {
        QFileInfo fileInfo(filename);

        QDir directory = fileInfo.dir();
        QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG"<< "*.png"<< "*.PNG",QDir::Files);
        foreach(QString filename, images) {
            if(filename.toLower().indexOf("right") >= 0){
                skyboxRightPicker->SetFileName(directory.absoluteFilePath(filename));
                skyboxStrings[0] = directory.absoluteFilePath(filename).toStdString();
            }
            if(filename.toLower().indexOf("left") >= 0){
                skyboxLeftPicker->SetFileName(directory.absoluteFilePath(filename));
                skyboxStrings[1] = directory.absoluteFilePath(filename).toStdString();
            }
            if(filename.toLower().indexOf("top") >= 0){
                skyboxTopPicker->SetFileName(directory.absoluteFilePath(filename));
                skyboxStrings[2] = directory.absoluteFilePath(filename).toStdString();
            }
            if(filename.toLower().indexOf("bottom") >= 0){
                skyboxBottomPicker->SetFileName(directory.absoluteFilePath(filename));
                skyboxStrings[3] = directory.absoluteFilePath(filename).toStdString();
            }
            if(filename.toLower().indexOf("front") >= 0){
                skyboxFrontPicker->SetFileName(directory.absoluteFilePath(filename));
                skyboxStrings[4] = directory.absoluteFilePath(filename).toStdString();
            }
            if(filename.toLower().indexOf("back") >= 0){
                skyboxBackPicker->SetFileName(directory.absoluteFilePath(filename));
                skyboxStrings[5] = directory.absoluteFilePath(filename).toStdString();
            }               
        }
        if(IsAllPlanes()) {
            emit SkyboxFileNames(skyboxStrings);
        }
    }


}