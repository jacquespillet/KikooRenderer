#include "RenderingPreferences.hpp"
#include "PreferencesWindow.hpp"
#include "3DEngine/Scene.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer 
{
RenderingPreferences::RenderingPreferences(PreferencesWindow* mainPrefWindow) {
    this->mainPrefWindow = mainPrefWindow;

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    setLayout(mainLayout);

    //0. Transparency
    // -------------------------------------------------------------------
    QGroupBox* transparencyGroupBox = new QGroupBox("Transparency");
    QVBoxLayout* transparencyLayout = new QVBoxLayout();
    transparencyGroupBox->setLayout(transparencyLayout);
    
    QCheckBox* enableTransparencyCheckBox = new QCheckBox("Enable");
    enableTransparencyCheckBox->setChecked(true);
    transparencyLayout->addWidget(enableTransparencyCheckBox);

    connect(enableTransparencyCheckBox, &QCheckBox::stateChanged, this, [this](int state) {
        scene = this->mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        GETGL
        
        if(state > 0) ogl->glEnable(GL_BLEND);
        else ogl->glDisable(GL_BLEND);

        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });

	//Blend Mode
	QHBoxLayout* blendModeLayout = new QHBoxLayout();
	QComboBox* blendModeList = new QComboBox();
	QLabel* blendModeLabel = new QLabel("Blend Mode");
    blendModeList->addItem("OneMinusSrcAlpha");
    blendModeList->addItem("Src Color");
    blendModeList->addItem("Dst Color");
	blendModeList->setCurrentIndex(0);

	blendModeLayout->addWidget(blendModeLabel);
	blendModeLayout->addWidget(blendModeList);

	connect(blendModeList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this](int index) {
        scene = this->mainPrefWindow->mainWindow->view3D->view3DGL->scene;
        scene->glWindow->makeCurrent();
        GETGL
        
        if(index == 0) ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        else if(index == 1) ogl->glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
        else if(index == 2) ogl->glBlendFunc(GL_SRC_ALPHA, GL_DST_COLOR);

        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });
    transparencyLayout->addLayout(blendModeLayout);


	mainLayout->addWidget(transparencyGroupBox);
    // -------------------------------------------------------------------       
}

}