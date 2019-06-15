#pragma once
#include "Common/Common.h"
#include "Component.hpp"
#include "../Shader.hpp"
#include "../Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent : public Component {
    public:
        MaterialComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        void Recompute(){}
        
        void SetShader(Shader* shader);
        void SetupShaderUniforms(glm::dmat4 modelMatrix, glm::dmat4 viewMatrix, glm::dmat4 projectionMatrix, Scene* scene);

        Shader* shader;


        float influence;

        glm::vec4 albedo;
};
}
}