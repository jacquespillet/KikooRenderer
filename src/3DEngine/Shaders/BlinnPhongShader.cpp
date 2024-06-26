#include "BlinnPhongShader.hpp"
#include "3DEngine/Scene.hpp"


#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer
{
namespace CoreEngine
{

/*
* Returns a layout containing all the inputs for blinn phong shading
*/
 QLayout* BlinnPhongParams::GetLayout() {
    QVBoxLayout* shaderParamsLayout = new QVBoxLayout();

    
    ColorPicker* specularColorPicker = new ColorPicker("specular Color", 210, 15, 60, 255);
    shaderParamsLayout->addWidget(specularColorPicker);
    connect(specularColorPicker, &ColorPicker::ColorPicked, this, [this](QColor color) {
        glm::vec4 specularColorVec = glm::vec4(color.red(), color.green(), color.blue(), color.alpha()) * 0.00392156f;
        specularColor = specularColorVec;
        scene->triggerRefresh = true;
    });

    // Normal Map
    TexturePicker* normalMapPicker = new TexturePicker("Normal Map", normalMapStr);
    shaderParamsLayout->addWidget(normalMapPicker);
    connect(normalMapPicker, &TexturePicker::FileModified, this, [this](QString string) {
        normalMapStr = string.toStdString();
        shouldLoadNormal = true;
        scene->triggerRefresh = true;
    });

    CustomSlider* normalMapInfluenceSlider = new CustomSlider(0.0f, 3.0f, 0.01, "Normal Map Influence", normalMapInfluence);
    shaderParamsLayout->addLayout(normalMapInfluenceSlider);
    QObject::connect(normalMapInfluenceSlider, &CustomSlider::Modified, [this](double val) {
        normalMapInfluence = val;
        scene->triggerRefresh = true;
    });

    //Specular Map
    TexturePicker* specularPicker = new TexturePicker("Specular Map", specularMapStr);
    shaderParamsLayout->addWidget(specularPicker);
    connect(specularPicker, &TexturePicker::FileModified, this, [this](QString string) {
        specularMapStr = string.toStdString();
        shouldLoadSpecular = true;
        scene->triggerRefresh = true;
    });


    //Height Map
    TexturePicker* heightPicker = new TexturePicker("height Map", heightMapStr);
    shaderParamsLayout->addWidget(heightPicker);
    connect(heightPicker, &TexturePicker::FileModified, this, [this](QString string) {
        scene->glWindow->makeCurrent();
        heightMapStr = string.toStdString();
        heightMap = Texture(heightMapStr, GL_TEXTURE3, false);
        scene->triggerRefresh = true;
        scene->glWindow->doneCurrent();
    });    
    
    CustomSlider* heightMapInfluenceSlider = new CustomSlider(0, 0.3, 0.001, "height Map Influence", heightMapInfluence);
    shaderParamsLayout->addLayout(heightMapInfluenceSlider);
    QObject::connect( heightMapInfluenceSlider, &CustomSlider::Modified, [this](double val) {
        heightMapInfluence = val;
        scene->triggerRefresh = true;
    });

	//Shader
	QHBoxLayout* parallaxModeLayout = new QHBoxLayout();
	QComboBox* parallaxModeList = new QComboBox();
	QLabel* parallaxModeLabel = new QLabel("parallax Mode");
    parallaxModeList->addItem("Simple Offset");
    parallaxModeList->addItem("RayMarching interpolation");
    parallaxModeList->addItem("RayMarching Binary Search");
	parallaxModeList->setCurrentIndex(2);

	parallaxModeLayout->addWidget(parallaxModeLabel);
	parallaxModeLayout->addWidget(parallaxModeList);

	shaderParamsLayout->addLayout(parallaxModeLayout);
	connect(parallaxModeList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this](int index) {
        this->parallaxMode = (PARALLAX_MODE) index;
        scene->triggerRefresh = true;
    });    

    CustomSlider* ambientSlider = new CustomSlider(0, 1, 0.01, "Ambient Factor", ambientFactor);
    shaderParamsLayout->addLayout(ambientSlider);
    QObject::connect( ambientSlider, &CustomSlider::Modified, [this](double val) {
        ambientFactor = val;
        scene->triggerRefresh = true;
    });

    CustomSlider* diffuseSlider = new CustomSlider(0, 1, 0.01, "Diffuse Factor", diffuseFactor);
    shaderParamsLayout->addLayout(diffuseSlider);
    QObject::connect(diffuseSlider, &CustomSlider::Modified, [this](double val) {
        diffuseFactor = val;
        scene->triggerRefresh = true;
    });

    CustomSlider* specularSlider = new CustomSlider(0, 1, 0.01, "Specular Factor", specularFactor);
    shaderParamsLayout->addLayout(specularSlider);
    QObject::connect(specularSlider, &CustomSlider::Modified, [this](double val) {
        specularFactor = val;
        scene->triggerRefresh = true;
    });

    CustomSlider* smoothnessSlider = new CustomSlider(0, 128, 1, "Smoothness", smoothness);
    shaderParamsLayout->addLayout(smoothnessSlider);
    QObject::connect(smoothnessSlider, &CustomSlider::Modified, [this](double val) {
        smoothness = val;
        scene->triggerRefresh = true;
    });

    QCheckBox* ReflectSkyboxCheckbox = new QCheckBox("Reflect/Refract Skybox");
    shaderParamsLayout->addWidget(ReflectSkyboxCheckbox);
    connect(ReflectSkyboxCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
        reflectSkybox = (state > 0);
        scene->triggerRefresh = true;
    });

    return shaderParamsLayout;
}

void BlinnPhongParams::SetUniforms() {
    GETGL
    /*
    * When textures changed in the form, load them for the incoming frame
    */
    if (shouldLoadNormal ) {
        normalMap = KikooRenderer::CoreEngine::Texture(normalMapStr, GL_TEXTURE2, false);
        shouldLoadNormal = false;
    }

    if (shouldLoadSpecular ) {
        specularMap = KikooRenderer::CoreEngine::Texture(specularMapStr, GL_TEXTURE1);
        shouldLoadSpecular = false;
    }

    /*
    Set variables for normal mapping : 
        * Texture variable
        * bool hasNormalTex
        * Influence of normals of the map on the actual normal
    */
    if(normalMap.loaded) {
        normalMap.Use();
        int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalTexture"); 
        ogl->glUniform1i(texLocation, 2); 
        
        int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
        ogl->glUniform1i(hasNormalTexLocation, 1);

        int normalMapInfluenceLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalMapInfluence");
        ogl->glUniform1f(normalMapInfluenceLocation, normalMapInfluence);

    }  else {
        int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
        ogl->glUniform1i(hasNormalTexLocation, 0);
    } 

    /*
    Set variables for Specular mapping : 
        * Texture variable
        * bool hasSpecularTex
    */
    if(specularMap.loaded) {
        specularMap.Use();
        int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularTexture"); 
        ogl->glUniform1i(texLocation, 1); 

        int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
        ogl->glUniform1i(hasSpecularTexLocation, 1);
    } else {
        int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
        ogl->glUniform1i(hasSpecularTexLocation, 0);
    }

    /*
    Set variables for Bump mapping : 
        * Texture variable
        * bool hasHeightMap
    */
    if(heightMap.loaded) {
        heightMap.Use();
        int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "heightTexture"); 
        ogl->glUniform1i(texLocation, 3); 

        int hasheightTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasHeightTex"); 
        ogl->glUniform1i(hasheightTexLocation, 1);

        int heightMapInfluenceLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "heightMapInfluence");
        ogl->glUniform1f(heightMapInfluenceLocation, heightMapInfluence);

        int parallaxModeLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "parallaxMode");
        ogl->glUniform1i(parallaxModeLocation, (int)parallaxMode);
    } else {
        int hasheightTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasHeightTex"); 
        ogl->glUniform1i(hasheightTexLocation, 0);
    }    

    int reflectSkyboxInt = reflectSkybox ? 1 : 0;
    int reflectSkyboxLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "reflectSkybox"); 
    ogl->glUniform1i(reflectSkyboxLocation, reflectSkyboxInt);
    if (reflectSkybox) {
    	MaterialComponent* skyboxMaterial = scene->skyboxCube->GetComponent<MaterialComponent>();
    	ogl->glActiveTexture(GL_TEXTURE6);
        skyboxMaterial->cubemap.Use();
    	int cubemapLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "cubemapTexture");
    	ogl->glUniform1i(cubemapLocation, 6);
    }

    /*
    * Set all factors for computing blinn phong shading
    */
    
    int specularColorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularColor"); 
    ogl->glUniform4fv(specularColorLocation, 1, glm::value_ptr(specularColor));

    int ambientFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "ambientFactor");
    ogl->glUniform1f(ambientFactorLocation, ambientFactor);

    int diffuseFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "diffuseFactor");
    ogl->glUniform1f(diffuseFactorLocation, diffuseFactor);

    int specularFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularFactor");
    ogl->glUniform1f(specularFactorLocation, specularFactor);

    int smoothnessLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "smoothness");
    ogl->glUniform1f(smoothnessLocation, smoothness);
}

/*
* Returns the actual blinn phong shader
*/
Shader GetBlinnPhongShader() {
    Shader blinnPhongShader;

    std::ifstream t("Shaders/Standard/BlinnPhong.vert");
    std::stringstream vertBuffer;
    vertBuffer << t.rdbuf();
    blinnPhongShader.vertSrc= vertBuffer.str();

    
    t = std::ifstream ("Shaders/Standard/BlinnPhong.frag");
    std::stringstream fragBuffer;
    fragBuffer << t.rdbuf();
    blinnPhongShader.fragSrc= fragBuffer.str();
    
    blinnPhongShader.SetId(SHADER_IDS::BLINNPHONG);
    blinnPhongShader.name = "Blinn Phong";
    std::cout << "StandardShaders:Compile: Compiling blinnPhongShader" << std::endl; 

    t.close();
    return blinnPhongShader;
}
}
}
