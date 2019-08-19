#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "3DEngine/Object3D.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    // //Maybe use it when different types of components ? 
    // class BoundingComponent : public Component {

    // }

    class BoundingBoxComponent : public Component {
        public:
        BoundingBoxComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        void Recompute(){}

        void GetWorldBounds(glm::dvec3* _min, glm::dvec3* _max);
        void GetLocalBounds(glm::dvec3* _min, glm::dvec3* _max);

        glm::dvec3 min;
        glm::dvec3 max;

        glm::dvec3 wmin;
        glm::dvec3 wmax;

		std::vector<glm::dvec3> positions;

        Object3D* GetBoxObject();
        private: 
        Object3D* boxObject;
        TransformComponent* transform;

    
		QJsonObject ToJSON() {
			QJsonObject json;
            json["type"] = "BoundingBox";

            QJsonObject minJson;
            minJson["x"] = min.x;
            minJson["y"] = min.y;
            minJson["z"] = min.z;
            json["min"] = minJson;

            QJsonObject maxJson;
            maxJson["x"] = max.x;
            maxJson["y"] = max.y;
            maxJson["z"] = max.z;
            json["max"] = maxJson;



			return json;
		}
    };
    
    // class BoundingSphere : public Component {
        
    // }

    // class BoundingEllipsoid : public Component {
        
    // }        
}
}