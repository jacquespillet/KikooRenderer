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

        void GetWorldBounds(glm::vec3* _min, glm::vec3* _max);
        void GetLocalBounds(glm::vec3* _min, glm::vec3* _max);
        
        ComponentInspector* GetInspector(){return nullptr;}
        
        void Refresh();

        glm::vec3 min;
        glm::vec3 max;

        glm::vec3 wmin;
        glm::vec3 wmax;

		std::vector<glm::vec3> positions;

        Object3D* GetBoxObject();

        bool shouldRefresh = false;
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