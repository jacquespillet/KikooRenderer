#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;

enum TILE_TYPE {WATER, DIRECTIONAL};
class WaterTile_2 : public Object3D {
public: 

    WaterTile_2(std::string name, Scene* scene, TILE_TYPE type = TILE_TYPE::DIRECTIONAL);

    void Start() override;
    void Enable() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;
    std::vector<QWidget*> GetInspectorWidgets() override;



    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "WaterTile_2";
        json["tileType"] = (int) tileType;
        json["tiling"] = tiling;
        json["strength"] = strength;
        json["constantHeightScale"] = constantHeightScale;
        json["modulatedHeightScale"] = modulatedHeightScale;
        json["gridResolution"] = gridResolution;
        json["modulatedTiling"] = modulatedTiling;
        json["dualGrid"] = dualGrid;

        json["distortionTiling"] = distortionTiling;
        json["distortionSpeed"] = distortionSpeed;
        json["distortionStrength"] = distortionStrength;
        json["flowOffset"] = flowOffset;
        json["distortionConstantHeightScale"] = distortionConstantHeightScale;
        json["distortionModulatedHeightScale"] = distortionModulatedHeightScale;
        json["uvJumpX"] = uvJump.x;
        json["uvJumpY"] = uvJump.y;
        return json;        
    }   
private:
    Object3D* quad;    
    MaterialComponent* quamaterial;
    Shader waterShader;

    Texture colorTexture;    
    Texture flowMap;    
    Texture normalMap;
    TILE_TYPE tileType = TILE_TYPE::DIRECTIONAL;

    //Directional parameters 
    //______________________
    float tiling = 1;
    float speed = 1;
    float strength = 0.1;
    float constantHeightScale = 0.5;
    float modulatedHeightScale = 10;
    float gridResolution = 30;
    float modulatedTiling = 50;
    int dualGrid = 1;


    //Distortion parameters 
    //______________________
    glm::vec2 uvJump = glm::vec2(0.0, 0.0);
    float distortionTiling = 3;
    float distortionSpeed = 0.5;
    float distortionStrength = 0.1;
    float flowOffset = 0;
    float distortionConstantHeightScale = 0.25;
    float distortionModulatedHeightScale = 0.75;
   

};

}
}