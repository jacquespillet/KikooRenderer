#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
class BackgroundPreferences : public QGroupBox {
Q_OBJECT
public:
    BackgroundPreferences();
    
    FilePicker* skyboxRightPicker;
    FilePicker* skyboxLeftPicker;
    FilePicker* skyboxTopPicker;
    FilePicker* skyboxBottomPicker;
    FilePicker* skyboxFrontPicker;
    FilePicker* skyboxBackPicker;

    std::vector<std::string> skyboxStrings;

protected:
    bool IsAllPlanes();
    void LookForSkybox(QString filename);

signals:
    void SkyboxFileNames(std::vector<std::string> filenames);
};
}