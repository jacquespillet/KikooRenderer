#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer {

namespace CoreEngine {
class Component;
class TransformComponent;
class LightComponent;
class Scene;
class Object3D {
    public: 
        std::string name;
        std::vector<Component*> components;
		TransformComponent* transform;

        Scene* scene;
        std::vector<Object3D*> childObjects;
        Object3D* parent = nullptr;


        bool visible = true;
        bool enabled;
        bool started;
        bool depthTest = true;
		bool isStatic = true;

        bool hasToRecompute=false;

        bool isSelected = false;

        bool castShadow = true;

        bool faceCamera = false;


        Object3D(std::string name, Scene* scene);
        ~Object3D();

        void AddComponent(Component* component);
        std::string AddObject(Object3D* object);
		void ClearObjects();

        // Component*  GetComponent(std::string name);
        
        template <typename T>
        T*  GetComponent(){
            for(int i=0; i<components.size(); i++) {
                T* comp = dynamic_cast<T*>(components[i]); 
                if (comp) {
                    return comp;
                }
            }
            return nullptr;
        }

        virtual void Start();
        virtual void Enable();
        virtual void Render(glm::mat4* overrideViewMatrix = nullptr);
        virtual void DepthRenderPass(LightComponent* light);
        virtual void Update();
        virtual void Destroy();
        virtual void Recompute();
        virtual void WindowResize(int w, int h);
        virtual std::vector<QWidget*> GetInspectorWidgets();


        virtual Object3D* Intersects(Geometry::Ray ray, double& distance);

        QJsonObject ToJSON();
        static Object3D* FromJSON(QJsonObject json, Scene* scene);

		void SetLayerMask(uint16_t _layerMask);
		uint16_t GetLayerMask();
        bool MatchesMask(uint16_t otherMask);

        void ToggleEditing();


    protected:
        uint16_t layerMask = 1;

        bool isEdit = false;
        std::vector<Object3D*> editingObjects;        
};
}

}