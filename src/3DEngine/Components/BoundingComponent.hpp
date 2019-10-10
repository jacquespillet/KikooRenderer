#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "3DEngine/Object3D.hpp"


namespace KikooRenderer {
namespace CoreEngine {
    // //Maybe use it when different types of bounding components ? 
    // class BoundingComponent : public Component {

    // }

    class BoundingBoxComponent;

    class BoundingBoxInspector : public ComponentInspector {
        Q_OBJECT
        public:
            BoundingBoxInspector(BoundingBoxComponent* boundingBoxComponent);
            BoundingBoxComponent* boundingBoxComponent;
            Scene* scene;

            QVBoxLayout* mainLayout;
            void Refresh();
    };

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
        
        ComponentInspector* GetInspector();
        
        void Refresh();

        glm::vec3 min;
        glm::vec3 max;

        glm::vec3 wmin;
        glm::vec3 wmax;

		std::vector<glm::vec3> positions;

        Object3D* GetBoxObject();

        bool shouldRefresh = false;

        bool isShown = false;

		QJsonObject ToJSON();
        static void FromJSON(QJsonObject json, Object3D* obj);
    
    private: 
        Object3D* boxObject;
        TransformComponent* transform;
		BoundingBoxInspector* boundingBoxInspector;
    };
    
    // class BoundingSphere : public Component {
        
    // }

    // class BoundingEllipsoid : public Component {
        
    // }        
}
}