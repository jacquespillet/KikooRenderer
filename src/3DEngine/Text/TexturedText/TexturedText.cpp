#pragma once
#include "TexturedText.hpp"

#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"
#include "../../Components/MeshFilterComponent.hpp"
#include "3DEngine/CameraScene.hpp"


namespace KikooRenderer {

namespace CoreEngine {

TexturedText::TexturedText(std::string name, Scene* scene) : Object3D(name, scene) {
    quads = GetQuad(scene, "Text", glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
    quads->Start();
    quads->Enable();

    quadsmaterial = quads->GetComponent<MaterialComponent>();
    mesh = quads->GetComponent<MeshFilterComponent>();

    camera = scene->camera;

    InitMetaFile("resources/Fonts/arial.fnt", "resources/Fonts/arial.png");

    transform = quads->transform;
}

void TexturedText::SetText(std::string text) {
    std::vector<Text::Line> lines = CreateStructure(text);
    TextMeshData data = CreateVertices(text, lines);

    std::vector<glm::vec3> vertex;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec4> colors;
    std::vector<int> triangles;
    for(int i=0; i<data.vertexPositions.size(); i+=2) {
        glm::vec3 vert(data.vertexPositions[i], data.vertexPositions[i+1], -0.0001 * i);
        vertex.push_back(vert);
        normals.push_back(glm::vec3(0, 0, 1));
        colors.push_back(glm::vec4(1));
        
        glm::vec2 coord(data.textureCoords[i], 1 - data.textureCoords[i+1]);
        uv.push_back(coord);
    }

    for(int i=0; i<vertex.size(); i++) {
        triangles.push_back(i);
    }
    mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, true);
    mesh->meshType = PRIMITIVE_MESH::QUAD_MESH;
    mesh->RebuildBuffers();

    Texture albedo = Texture("resources/Fonts/arial.png", GL_TEXTURE0);
    quadsmaterial->SetAlbedoTex(albedo);
}

std::vector<Text::Line> TexturedText::CreateStructure(std::string text) {

    std::vector<Text::Line> lines;
    Text::Line currentLine(mf->getSpaceWidth(), fontSize, maxLineSize);
    std::cout << mf->getSpaceWidth() << std::endl;
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
    mf = new Text::MetaFile(fontFile, imageFile, LINE_HEIGHT, DESIRED_PADDING);     
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
}

std::vector<QWidget*> TexturedText::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    return res;
}


void TexturedText::Render(glm::mat4* overrideViewMatrix) {
    GETGL
    quads->Render();
    quadsmaterial->firstIter = false;
}

void TexturedText::LateRender() {
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