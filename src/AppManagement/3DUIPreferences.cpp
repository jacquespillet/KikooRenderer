#include "3DUIPreferences.hpp"

namespace KikooRenderer {
    UI3DPreferences::UI3DPreferences(): QGroupBox("3D GUI") {
        
        QVBoxLayout* GUI3DPreferencesLayout = new QVBoxLayout();
        setLayout(GUI3DPreferencesLayout);

        QCheckBox* checkAll  = new QCheckBox("Check/Uncheck All");
        GUI3DPreferencesLayout->addWidget(checkAll);

        QCheckBox* renderGridCheckbox  = new QCheckBox("Render Grid");
        GUI3DPreferencesLayout->addWidget(renderGridCheckbox);
        
        QCheckBox* renderTransformwidgetCheckbox  = new QCheckBox("Render Transform Widget");
        GUI3DPreferencesLayout->addWidget(renderTransformwidgetCheckbox);
        
        QCheckBox* renderAxesCheckbox  = new QCheckBox("Render Axes");
        GUI3DPreferencesLayout->addWidget(renderAxesCheckbox);
    }
}