#pragma once
#include "TexturedText.hpp"

#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"
#include "../../Components/MeshFilterComponent.hpp"
#include "3DEngine/CameraScene.hpp"


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
    std::vector<Text::Line> lines = CreateStructure(text);
    TextMeshData data = CreateVertices(text, lines);
}

std::vector<Text::Line> TexturedText::CreateStructure(std::string text) {
    int fontSize = 1;
    int maxLineSize = 10;

    std::vector<Text::Line> lines;
    Text::Line currentLine(mf->getSpaceWidth(), fontSize, maxLineSize);
    Text::Word currentWord(fontSize);
    for(char& c : text) {
        int ascii = (int) c;
        if(ascii == SPACE_ASCII) {   
            bool added = currentLine.attemptToAddWord(currentWord);
            if(!added) {
                lines.push_back(currentLine);
                currentLine = Text::Line(mf->getSpaceWidth(), fontSize, maxLineSize);
                currentLine.attemptToAddWord(currentWord);
            }
            currentWord = Text::Word(fontSize);
            continue;
        }

        Text::Character character = (*mf->getCharacter(ascii));
        currentWord.addCharacter(character);
    }

    bool added = currentLine.attemptToAddWord(currentWord);
    if (!added) {
        lines.push_back(currentLine);
        currentLine = Text::Line(mf->getSpaceWidth(), fontSize, maxLineSize);
        currentLine.attemptToAddWord(currentWord);
    }
    lines.push_back(currentLine);    

    return lines;
}

TexturedText::TextMeshData TexturedText::CreateVertices(std::string text, std::vector<Text::Line> lines) {
    int fontSize = 1;
    int maxLineSize = 10;

    TexturedText::TextMeshData data;
    int numLines = lines.size();
    
    double curserX = 0;
    double curserY = 0;
    std::vector<float> vertices;
    std::vector<float> textureCoords;
    for (int i=0; i<numLines; i++) {
        Text::Line line = lines[i];
        if (centerText) {
            curserX = (line.getMaxLength() - line.getLineLength()) / 2;
        }
        for (int j=0; j<line.getWords().size(); j++) {
            Text::Word word = line.getWords()[j];
            for (int k=0; k<word.getCharacters().size(); k++) {
                Text::Character letter = word.getCharacters()[k];
                AddVerticesForCharacter(curserX, curserY, letter, fontSize, vertices);
                AddTexCoords(textureCoords, letter.getxTextureCoord(), letter.getyTextureCoord(),
                        letter.getXMaxTextureCoord(), letter.getYMaxTextureCoord());
                curserX += letter.getxAdvance() * fontSize;
            }
            curserX += mf->getSpaceWidth() * fontSize;
        }
        curserX = 0;
        curserY += LINE_HEIGHT * fontSize;
    }

    data.textureCoords = textureCoords;	    
    data.vertexPositions = vertices;
    return data;
}

void TexturedText::AddVerticesForCharacter(double curserX, double curserY, Text::Character character, double fontSize,std::vector<float>& vertices) {
    double x = curserX + (character.getxOffset() * fontSize);
    double y = curserY + (character.getyOffset() * fontSize);
    double maxX = x + (character.getSizeX() * fontSize);
    double maxY = y + (character.getSizeY() * fontSize);
    double properX = (2 * x) - 1;
    double properY = (-2 * y) + 1;
    double properMaxX = (2 * maxX) - 1;
    double properMaxY = (-2 * maxY) + 1;
    AddVertices(vertices, properX, properY, properMaxX, properMaxY);
}

void TexturedText::AddVertices(std::vector<float>& vertices, double x, double y, double maxX, double maxY) {
    vertices.push_back((float) x);
    vertices.push_back((float) y);
    vertices.push_back((float) x);
    vertices.push_back((float) maxY);
    vertices.push_back((float) maxX);
    vertices.push_back((float) maxY);
    vertices.push_back((float) maxX);
    vertices.push_back((float) maxY);
    vertices.push_back((float) maxX);
    vertices.push_back((float) y);
    vertices.push_back((float) x);
    vertices.push_back((float) y);
}

void TexturedText::AddTexCoords(std::vector<float>& texCoords, double x, double y, double maxX, double maxY) {
    texCoords.push_back((float) x);
    texCoords.push_back((float) y);
    texCoords.push_back((float) x);
    texCoords.push_back((float) maxY);
    texCoords.push_back((float) maxX);
    texCoords.push_back((float) maxY);
    texCoords.push_back((float) maxX);
    texCoords.push_back((float) maxY);
    texCoords.push_back((float) maxX);
    texCoords.push_back((float) y);
    texCoords.push_back((float) x);
    texCoords.push_back((float) y);
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