#include "3DUIPreferences.hpp"
#include "PreferencesWindow.hpp"

namespace KikooRenderer {
    UI3DPreferences::UI3DPreferences(PreferencesWindow* mainPrefWindow): QGroupBox("3D GUI") {

        this->mainPrefWindow = mainPrefWindow;
        
        QVBoxLayout* GUI3DPreferencesLayout = new QVBoxLayout();
        setLayout(GUI3DPreferencesLayout);

        QCheckBox* checkAll  = new QCheckBox("Check/Uncheck All");
        checkAll->setChecked(true);
        GUI3DPreferencesLayout->addWidget(checkAll);

        QCheckBox* renderGridCheckbox  = new QCheckBox("Render Grid");
        renderGridCheckbox->setChecked(true);
        GUI3DPreferencesLayout->addWidget(renderGridCheckbox);
        
        QCheckBox* renderTransformwidgetCheckbox  = new QCheckBox("Render Transform Widget");
        renderTransformwidgetCheckbox->setChecked(true);
        GUI3DPreferencesLayout->addWidget(renderTransformwidgetCheckbox);
        
        QCheckBox* renderAxesCheckbox  = new QCheckBox("Render Axes");
        renderAxesCheckbox->setChecked(true);
        GUI3DPreferencesLayout->addWidget(renderAxesCheckbox);
        
        QCheckBox* renderAxesHelperCheckbox  = new QCheckBox("Render Axes Helper");
        renderAxesHelperCheckbox->setChecked(true);
        GUI3DPreferencesLayout->addWidget(renderAxesHelperCheckbox);

        connect(checkAll, &QCheckBox::stateChanged, this, [this, renderGridCheckbox, renderTransformwidgetCheckbox, renderAxesCheckbox, renderAxesHelperCheckbox](int state) {
            renderGridCheckbox->setChecked(state > 0);
            renderTransformwidgetCheckbox->setChecked(state > 0);
            renderAxesCheckbox->setChecked(state > 0);
            renderAxesHelperCheckbox->setChecked(state > 0);

            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderGrid = state > 0;
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderTransformWidget = state > 0;
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderAxes = state > 0;

            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->triggerRefresh = true;
        });

        connect(renderGridCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderGrid = state > 0;
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->triggerRefresh = true;
        });

        connect(renderTransformwidgetCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderTransformWidget = state > 0;
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->triggerRefresh = true;
        });

        
        connect(renderAxesCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderAxes = state > 0;
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->triggerRefresh = true;
        });
        
        
        connect(renderAxesHelperCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->renderAxesHelper = state > 0;
            this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->triggerRefresh = true;
        });

    }
}