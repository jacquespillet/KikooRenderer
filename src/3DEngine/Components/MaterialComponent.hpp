#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "3DEngine/Shader.hpp"
#include "3DEngine/Texture.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;


class MaterialInspector : public QGroupBox {
	Q_OBJECT
	public:
		MaterialInspector(MaterialComponent* materialComponent);
		MaterialComponent* materialComponent;
		Scene* scene;

		QVBoxLayout* mainLayout;
		QVBoxLayout* shaderParametersLayout;

		void Refresh();
		void UpdateShaderParameters();

};

class MaterialComponent : public Component {
    public:
        MaterialComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

		MaterialInspector* GetInspector();
        
        void SetShader(Shader* shader);
        void SetupShaderUniforms(glm::dmat4 modelMatrix, glm::dmat4 viewMatrix, glm::dmat4 projectionMatrix, Scene* scene);

        Shader* shader;

        float influence;

        glm::vec4 albedo;

        Texture albedoTex;
        Texture specularTex;
        Texture normalTex;

		std::string albedoTexStr = "";
		std::string specularTexStr = "";
		std::string normalTexStr = "";

		float ambientFactor;
		float diffuseFactor;
		float specularFactor;
		int smoothness;

		MaterialInspector* materialInspector;
};
}
}