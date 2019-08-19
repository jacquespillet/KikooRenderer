#pragma once
#include "Util/Common.h"
#include "Component.hpp"


namespace KikooRenderer {
namespace CoreEngine {
class LightComponent : public Component {
    public:
        LightComponent(Object3D* object, glm::dvec4 color, glm::dvec3 attenuation, int type);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        void Recompute(){}

        int type; //0 directional, 1 point, 2 spot
        glm::dvec3 attenuation;
        glm::dvec4 color;
		double fov;

    
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