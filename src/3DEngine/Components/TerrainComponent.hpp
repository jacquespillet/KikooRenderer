
#pragma once 
#include "Util/Common.h"
#include "Component.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class MeshFilterComponent;
class TerrainComponent : public Component {
    public: 
        TerrainComponent(Object3D* object, float width, float height, int subdivX, int subdivY);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

		QJsonObject ToJSON();
    private:
        int width;
        int height;
        int subdivX;
        int subdivY;
        MeshFilterComponent* mesh;

        int resolution = 100;

        float GetXDerivative(int x, int z, int vInx);
        float GetZDerivative(int x, int z, int vInx);
};

}
}