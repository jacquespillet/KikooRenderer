#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
class BackgroundPreferences : public QGroupBox {
Q_OBJECT
public:
    BackgroundPreferences();
    
    TexturePicker* skyboxRightPicker;
    TexturePicker* skyboxLeftPicker;
    TexturePicker* skyboxTopPicker;
    TexturePicker* skyboxBottomPicker;
    TexturePicker* skyboxFrontPicker;
    TexturePicker* skyboxBackPicker;

    std::vector<std::string> skyboxStrings;

protected:
    bool IsAllPlanes();
    void LookForSkybox(QString filename);

signals:
    void SkyboxFileNames(std::vector<std::string> filenames);
};
}