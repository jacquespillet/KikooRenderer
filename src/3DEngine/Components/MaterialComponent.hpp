#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "3DEngine/Shader.hpp"
#include "3DEngine/Texture.hpp"
#include "3DEngine/Cubemap.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;


class MaterialInspector : public ComponentInspector {
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

        ComponentInspector* GetInspector();
        
        void SetShader(Shader shader);
        void SetupShaderUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, Scene* scene);

        Shader shader;

        float influence;

        glm::vec4 albedo;

        Texture albedoTex;
		std::string albedoTexStr = "";
		bool shouldLoadAlbedo = false;

		void SetCubemap(std::vector<std::string> cubemapFilenames);
		void SetAlbedoTex(Texture tex);
		std::vector<std::string> cubemapfilenames;
        Cubemap cubemap;
		bool shouldLoadCubemap = false;

		bool flipNormals=false;

		bool receiveShadow = true;

		MaterialInspector* materialInspector;
		
		ShaderParams* params;

		bool firstIter = true;

		QJsonObject ToJSON();		
        static void FromJSON(QJsonObject json, Object3D* obj);
};
}
}