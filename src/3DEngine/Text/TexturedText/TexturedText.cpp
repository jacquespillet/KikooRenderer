#pragma once
#include "TexturedText.hpp"

#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"
#include "../../Components/MeshFilterComponent.hpp"
#include "3DEngine/CameraScene.hpp"

#include "Line.hpp"

namespace KikooRenderer {

namespace CoreEngine {

TexturedText::TexturedText(std::string name, Scene* scene) : Object3D(name, scene) {
    // lensFlareShader = GetLensFlareShader();

    quad = GetQuad(scene, "Text", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
    quad->Start();
    quad->Enable();


    quamaterial = quad->GetComponent<MaterialComponent>();
    // quamaterial->SetShader(lensFlareShader);

    mesh = quad->GetComponent<MeshFilterComponent>();

    camera = scene->camera;

    InitMetaFile("resources/Fonts/arial.fnt", "resources/Fonts/arial.png");

}

void TexturedText::SetText(std::string text) {
    CreateStructure(text);
}

void TexturedText::CreateStructure(std::string text) {
    int fontSize = 1;
    int maxLineSize = 10;

    std::vector<Text::Line> lines;
    Text::Line currentLine(mf->getSpaceWidth(), fontSize, maxLineSize);
    Text::Word currentWord(fontSize);
    for(char& c : text) {
        // do_things_with(c);
    }
    //STOPPED HEre
}

void TexturedText::InitMetaFile(std::string fontFile, std::string imageFile) {
    mf = new Text::MetaFile(fontFile, imageFile);   
}

void TexturedText::WindowResize(int w, int h) {
}


void TexturedText::Start() {
	started = true;
}

void TexturedText::Enable() {
	enabled = true;
}

void TexturedText::Update() {
    scene->triggerRefresh = true;
}

std::vector<QWidget*> TexturedText::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    return res;
}


void TexturedText::Render(glm::mat4* overrideViewMatrixp) {
 
}

void TexturedText::LateRender() {
   GETGL

//    if(scene->lightObjects.size() > 0) {
//         glm::mat4 mvpMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix() * scene->lightObjects[0]->transform->GetWorldModelMatrix();       
        
//         glm::vec4 projectedLightPos(0, 0, 0, 1);
//         projectedLightPos = mvpMatrix * projectedLightPos;
//         if(projectedLightPos.w > 0) {
//             projectedLightPos /= projectedLightPos.w;

//             glm::vec2 screenSpaceLightPos = glm::vec2(projectedLightPos.x, projectedLightPos.y);
//             glm::vec2 lightToCenter = -screenSpaceLightPos;

//             for(int i=0; i<textures.size(); i++) {
//                 glm::vec2 direction = glm::normalize(lightToCenter);
//                 direction *= (i * spacing);

//                 glm::vec3 position = glm::vec3(screenSpaceLightPos, -0.01 * i) + glm::vec3(direction, 0);
//                 glm::vec3 size(sizes[i], 1);   

//                 ogl->glUseProgram(lensFlareShader.programShaderObject);
//                 ogl->glUniform3fv(ogl->glGetUniformLocation(lensFlareShader.programShaderObject, "screenPosition"), 1, glm::value_ptr(position));
//                 ogl->glUniform3fv(ogl->glGetUniformLocation(lensFlareShader.programShaderObject, "size"), 1, glm::value_ptr(size));

//                 ogl->glActiveTexture(GL_TEXTURE0);
//                 ogl->glBindTexture(GL_TEXTURE_2D, textures[i].glTex);
//                 ogl->glUniform1i(ogl->glGetUniformLocation(lensFlareShader.programShaderObject, "albedoTexture"), 0);    

//             }    
//         }
//    }
    quad->Render();
    quamaterial->firstIter = false;
}

void TexturedText::DepthRenderPass(LightComponent* light) {
}

void TexturedText::Destroy() {
    delete mf;
}

void TexturedText::Recompute() {
}


}
}