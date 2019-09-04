#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "../Framebuffer.hpp"
#include "../CubeFramebuffer.hpp"
#include "../Shader.hpp"


namespace KikooRenderer {
namespace CoreEngine {

class LightComponent;


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

        int type; //0 directional, 1 point, 2 spot
        glm::dvec3 attenuation;
        glm::dvec4 color;
		double fov;
        

        //Directional Shadow Map
        Shader depthPassShader;
	    Framebuffer* depthFBO;
        glm::dmat4 lightProjection;
        glm::dmat4 lightSpaceMatrix;

        //Cubemap shadow map
        Shader cubeDepthPassShader;
	    CubeFramebuffer* depthCubeFBO;
        std::vector<glm::dmat4> lightSpaceMatrices;

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