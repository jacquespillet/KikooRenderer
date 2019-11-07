#pragma once
#include "Util/Common.h"
#include "../../Object3D.hpp"
#include "../../Shader.hpp"
#include "../../Framebuffer.hpp"

#include "MetaFile.hpp"

namespace KikooRenderer {

namespace CoreEngine {

class MaterialComponent;
class MeshFilterComponent;
class CameraScene;

class TexturedText : public Object3D {
public: 
    TexturedText(std::string name, Scene* scene);

    void Start() override;
    void Enable() override;
    void LateRender() override;
    void Render(glm::mat4* overrideViewMatrix=nullptr) override;
    void DepthRenderPass(LightComponent* light) override;
    void Update() override;
    void Destroy() override;
    void Recompute() override;
    void WindowResize(int w, int h) override;
    std::vector<QWidget*> GetInspectorWidgets() override;


    QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "TexturedText";
    }

    void SetText(std::string);

private:
    Object3D* quad;
    MaterialComponent* quamaterial;
    MeshFilterComponent* mesh;
    // Shader lensFlareShader;
    
    // std::vector<Texture> textures;
    // std::vector<glm::vec2> sizes;

    CameraScene* camera;

    Text::MetaFile* mf;

    void InitMetaFile(std::string fontFile, std::string imageFile);
    void CreateStructure(std::string text);
    void CompleteStructure();
    void CreateVertices();
    void AddVerticesForCharacter();
    void AddVertices();
    void AddTexCoords();


	std::string textString ="test";
	float fontSize = 1;

	int textMeshVao;
	int vertexCount;
	glm::vec3 colour =  glm::vec3(1.0f, 0.0f, 0.0f);

	glm::vec2 position;
	float lineMaxSize;
	int numberOfLines;

	// FontType font;
    std::string font;

	boolean centerText = false;
    
};

}
}