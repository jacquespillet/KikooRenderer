#pragma once
#include "Util/Common.h"
#include "BoundingComponent.hpp"
#include "Component.hpp"


namespace KikooRenderer {
namespace CoreEngine {
	class MeshFilterComponent;
    class FluidComponent : public Component {
        public:
        FluidComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

        MeshFilterComponent* meshFilter;

        ComponentInspector* GetInspector(){return nullptr;}
        
        int width = 100;
        int height = 100;
        int numVert;

        int renderBuffer=0;

        std::vector<glm::vec3> buffer[2];
        std::vector<glm::vec3> normal;
        std::vector<glm::vec3> tangent;

        float t = 0.1; //time interval
        float c = 0.4; //velocity
        float d = 0.1; //Distance btw vert
        float mu = 0.01;

        float k1, k2, k3;

    
		QJsonObject ToJSON() {
			QJsonObject json;
			json["name"] = "mName";
			json["level"] = "mLevel";
			json["classType"] = "mClassType";
			return json;
		}

        static void FromJSON(QJsonObject json, Object3D* obj) {
            
        }
    };
}
}