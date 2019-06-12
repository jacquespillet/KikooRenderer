#include "MeshFilterComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_2_Core>
#define GLV QOpenGLFunctions_3_2_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

MeshFilterComponent::MeshFilterComponent(Object3D* object) : Component("MeshFilter", object) {
    inited= false;
	drawingMode = GL_TRIANGLES;
	primitiveSize = 1;
}
void MeshFilterComponent::OnStart(){}
void MeshFilterComponent::OnEnable(){
    InitBuffers();
	RebuildBuffers();
}
void MeshFilterComponent::OnUpdate(){}
void MeshFilterComponent::OnRender(){
	GETGL

	//
	//bind VAO
	//
	
	ogl->glBindVertexArray(vertexArrayObject);
	
	//
	//Draw
	//
	
	if(drawingMode == GL_LINES) {
		// ogl->glLineWidth(10.0);
	}

	//draw elements
	ogl->glDrawElements(drawingMode, triangles.size(), GL_UNSIGNED_INT, (void*)0);
	
	//
	//unbind VAO
	//
	
	ogl->glBindVertexArray(0);
} 

void MeshFilterComponent::OnDestroy() {
	DestroyBuffers();
}


void MeshFilterComponent::LoadFromBuffers(std::vector<glm::dvec3> _vertex,
std::vector<glm::dvec3> _normals,
std::vector<glm::dvec2> _uv,
std::vector<glm::dvec4> _colors,
std::vector<int> _triangles) {
    this->triangles = _triangles;
    for(int i=0; i<_vertex.size(); i++) {
        Vertex vertex = {
            _vertex[i],
            _normals[i],
            _uv[i],
            _colors[i].r,
            _colors[i].g,
            _colors[i].b,
            _colors[i].a
        };
        vertices.push_back(vertex);
    }
}

void MeshFilterComponent::InitBuffers() {
    GETGL
	ogl->glGenBuffers(1, &vertexBuffer);
	ogl->glGenBuffers(1, &elementBuffer);
	ogl->glGenVertexArrays(1, &vertexArrayObject);
	
	//Bind VAO
	ogl->glBindVertexArray(vertexArrayObject);
	
	//bind buffers
	ogl->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	
	//set vertex attributes
	ogl->glEnableVertexAttribArray(0);
	ogl->glEnableVertexAttribArray(1);
	ogl->glEnableVertexAttribArray(2);
	ogl->glEnableVertexAttribArray(3);
	ogl->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)0));
	ogl->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)12));
	ogl->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)24));
	ogl->glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)((uintptr_t)32));
	
	//Unbind VAO
	ogl->glBindVertexArray(0);
	
	//Unbind array and element buffers
	ogl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	inited = true; 
}

void MeshFilterComponent::DestroyBuffers() { 
	GETGL
	if(inited)
	{
		ogl->glDeleteBuffers(1, &vertexBuffer);
		ogl->glDeleteBuffers(1, &elementBuffer);
		ogl->glDeleteVertexArrays(1, &vertexArrayObject);
		inited = false; 
	}
}

void MeshFilterComponent::RebuildBuffers() {
	GETGL
	//Bind VAO
	ogl->glBindVertexArray(vertexArrayObject);
	
	//Bind buffers
	ogl->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	
	//copy data to buffers
	ogl->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), (uint8_t*)&vertices[0], GL_STATIC_DRAW);
	ogl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(unsigned int), (uint8_t*)&triangles[0], GL_STATIC_DRAW);
	
	//Unbind VAO
	ogl->glBindVertexArray(0);
	
	//Unbind array and element buffers
	ogl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    
}

}
}