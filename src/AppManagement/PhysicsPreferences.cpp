#include "PhysicsPreferences.hpp"
#include "PreferencesWindow.hpp"

namespace KikooRenderer 
{
PhysicsPreferences::PhysicsPreferences(PreferencesWindow* mainPrefWindow) {
    this->mainPrefWindow = mainPrefWindow;

    //0. FXAA Post
    // -------------------------------------------------------------------
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

  
	//World Type
	QHBoxLayout* worldTypeLayout = new QHBoxLayout();
	QComboBox* worldTypeList = new QComboBox();
	QLabel* worldTypeLabel = new QLabel("World Type");
    worldTypeList->addItem("Rigid");
    worldTypeList->addItem("Soft");
    worldTypeList->addItem("Deformable");
	worldTypeList->setCurrentIndex(0);

	worldTypeLayout->addWidget(worldTypeLabel);
	worldTypeLayout->addWidget(worldTypeList);

	mainLayout->addLayout(worldTypeLayout);
	connect(worldTypeList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this](int index) {
        this->mainPrefWindow->mainWindow->view3D->view3DGL->scene->GetSimulation()->SetWorld((Physics::Bullet::WORLD_TYPE)index);
    });

    // -------------------------------------------------------------------       
}

}