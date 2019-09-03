#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "../Framebuffer.hpp"
#include "../Shader.hpp"


namespace KikooRenderer {
namespace CoreEngine {

class LightComponent;

//Might use later ? ...
// class LightViewBuilder {
//     public:
//     glm::dvec4 UP;
//     glm::dvec4 FORWARD;
//     double MAX_DISTANCE;
//     double OFFSET;

//     double farHeight;
//     double farWidth;
//     double nearHeight;
//     double nearWidth;

//     glm::dvec3 min;
//     glm::dvec3 max;

//     glm::dmat4 lightViewMatrix;

//     LightComponent* light;

//     LightViewBuilder(LightComponent* light);
//     void Update();

//     //Helper funcs
//     void calculateWidthsAndHeights(); 
//     glm::dmat4 calculateCameraRotationMatrix();
//     glm::dvec4 calculateLightSpaceFrustumCorner(glm::dvec3 startPoint, glm::dvec3 direction,float width);
//     std::vector<glm::dvec4> calculateFrustumVertices(glm::dmat4 rotation, glm::dvec3 forwardVector, glm::dvec3 centerNear, glm::dvec3 centerFar);

//     glm::dmat4 GetProjection();
//     glm::dmat4 GetView();

// };

class LightComponent : public Component {
    public:
        LightComponent(Object3D* object, glm::dvec4 color, glm::dvec3 attenuation, int type);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        void Recompute();

        void RenderDepthMap();

        // LightViewBuilder lightViewBuilder;

        int type; //0 directional, 1 point, 2 spot
        glm::dvec3 attenuation;
        glm::dvec4 color;
		double fov;
        
        Shader depthPassShader;

	    Framebuffer* depthFBO;
        glm::dmat4 lightProjection;
        glm::dmat4 lightSpaceMatrix;
    
		QJsonObject ToJSON() {
			QJsonObject json;
            json["type"] = "Light";
            json["lightType"] = type;

            QJsonObject attenuationJson;
            attenuationJson["x"] = attenuation.x;
            attenuationJson["y"] = attenuation.y;
            attenuationJson["z"] = attenuation.z;
            json["attenuation"] = attenuationJson;


            QJsonObject colorJson;
            colorJson["r"] = color.r;
            colorJson["g"] = color.g;
            colorJson["b"] = color.b;
            colorJson["a"] = color.a;
            json["color"] = colorJson;

            json["fov"] = fov;

			return json;
		}
};
}
}