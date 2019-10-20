#include "MeshFilterComponent.hpp"
#include "BaseMeshes.hpp"
#include "Util/ModelLoader.hpp"
#include "BoundingComponent.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {

std::string MeshFilterInspector::GetByPrimitiveMesh(PRIMITIVE_MESH type) {
	switch (type)
	{
		case MODEL_MESH:
			return "Model";
			break;
		case CUBE_MESH:
			return "Cube";
			break;
		case SPHERE_MESH:
			return "Sphere";
			break;
		case CIRCLE_MESH:
			return "Circle";
			break; 
		case CONE_MESH:
			return "Cone";
			break;       
		case QUAD_MESH:
			return "Quad";
			break;                                    
		default:
			return "";
			break;
	}
}
MeshFilterInspector::MeshFilterInspector(MeshFilterComponent* meshFilterComponent) : ComponentInspector("Mesh", meshFilterComponent) {
	this->meshFilterComponent = meshFilterComponent;
	Object3D* object = meshFilterComponent->object3D;
	scene = object->scene;
	mainLayout = new QVBoxLayout();

	QHBoxLayout* modelLayout = new QHBoxLayout();
	QComboBox* modelList = new QComboBox();
	QLabel* modelLabel = new QLabel("Model");
	for (int i = 0; i < PRIMITIVE_MESH::MESH_PRIMITIVE_ENUM_SIZE; i++) {
		std::string modelName = MeshFilterInspector::GetByPrimitiveMesh((PRIMITIVE_MESH)i);
		modelList->addItem(QString::fromStdString(modelName));
	}
	int modelInx = meshFilterComponent->meshType;
	modelList->setCurrentIndex(modelInx);
	modelLayout->addWidget(modelLabel);
	modelLayout->addWidget(modelList);
	mainLayout->addLayout(modelLayout);

	TexturePicker* modelPicker = new TexturePicker("Model File Name");
	mainLayout->addWidget(modelPicker);
	modelPicker->setVisible((meshFilterComponent->meshType == PRIMITIVE_MESH::MODEL_MESH));
	connect(modelPicker, &TexturePicker::FileModified, this, [this, meshFilterComponent, object](QString value) {
		meshFilterComponent->modelpath = value.toStdString();
		std::vector<glm::vec3> vertex;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec4> colors;
		std::vector<int> triangles;
		Util::FileIO::GetModelBuffers(meshFilterComponent->modelpath, &vertex, &normals, &uv, &colors, &triangles);
		
		meshFilterComponent->LoadFromBuffers(vertex, normals, uv, colors, triangles, true);
		meshFilterComponent->shoudRebuildBuffers = true;

		BoundingBoxComponent* bb = object->GetComponent<BoundingBoxComponent>();
		bb->shouldRefresh = true;
	});


	connect(modelList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this,meshFilterComponent, modelPicker, object](int index) {
		if(index != PRIMITIVE_MESH::MODEL_MESH) {
			std::vector<glm::vec3> vertex;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> uv;
			std::vector<glm::vec4> colors;
			std::vector<int> triangles;

			meshFilterComponent->meshType =(PRIMITIVE_MESH)index;
			if(index == PRIMITIVE_MESH::CUBE_MESH)    GetCubeBuffers(&vertex, &normals, &uv, &colors, &triangles);
			if(index == PRIMITIVE_MESH::CIRCLE_MESH)  GetCircleBuffers(&vertex, &normals, &uv, &colors, &triangles);
			if(index == PRIMITIVE_MESH::CONE_MESH)    GetConeBuffers(&vertex, &normals, &uv, &colors, &triangles);
			if(index == PRIMITIVE_MESH::QUAD_MESH)    GetQuadBuffers(&vertex, &normals, &uv, &colors, &triangles);
			if(index == PRIMITIVE_MESH::SPHERE_MESH)  GetSphereBuffers(&vertex, &normals, &uv, &colors, &triangles);

			meshFilterComponent->LoadFromBuffers(vertex, normals, uv, colors, triangles, false);
			meshFilterComponent->shoudRebuildBuffers = true;
		} else {
			modelPicker->setVisible(true);
		}
	});




	setLayout(mainLayout);
}

void MeshFilterInspector::Refresh() {

}


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
void MeshFilterComponent::OnUpdate(){
	if(shoudRebuildBuffers) {
		RebuildBuffers();
		shoudRebuildBuffers = false;
		object3D->scene->triggerRefresh = true;
	}
}
void MeshFilterComponent::OnRender(){
	GETGL
	//bind VAO	
	ogl->glBindVertexArray(vertexArrayObject);

	if (inited) {
		if(renderInstanced) {
			ogl->glDrawElementsInstanced(drawingMode, triangles.size(), GL_UNSIGNED_INT, 0, numInstances);
		} else {
			ogl->glDrawElements(drawingMode, triangles.size(), GL_UNSIGNED_INT, (void*)0);
		}
	}
	
	//unbind VAO
	ogl->glBindVertexArray(0);
	ogl->glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	ogl->glBindTexture(GL_TEXTURE_2D, 0);
} 

void MeshFilterComponent::OnDestroy() {
	DestroyBuffers();
}
void MeshFilterComponent::Recompute() {}

ComponentInspector* MeshFilterComponent::GetInspector() {
	meshFilterInspector = new MeshFilterInspector(this);
	return meshFilterInspector;

}


void MeshFilterComponent::CalculateTangents(std::vector<glm::vec4>& tangents ,std::vector<glm::vec3> _vertices,  std::vector<glm::vec3> normals, std::vector<glm::vec2> uv,std::vector<int> triangles) {
	std::vector<glm::vec4> tan1(_vertices.size(), glm::vec4(0));
	std::vector<glm::vec4> tan2(_vertices.size(), glm::vec4(0));
	for(uint64_t i=0; i<triangles.size(); i+=3) {
		glm::vec3 v1 = _vertices[triangles[i]];
		glm::vec3 v2 = _vertices[triangles[i + 1]];
		glm::vec3 v3 = _vertices[triangles[i + 2]];

		glm::vec2 w1 = uv[triangles[i]];
		glm::vec2 w2 = uv[triangles[i+1]];
		glm::vec2 w3 = uv[triangles[i+2]];

		double x1 = v2.x - v1.x;
		double x2 = v3.x - v1.x;
		double y1 = v2.y - v1.y;
		double y2 = v3.y - v1.y;
		double z1 = v2.z - v1.z;
		double z2 = v3.z - v1.z;

		double s1 = w2.x - w1.x;
		double s2 = w3.x - w1.x;
		double t1 = w2.y - w1.y;
		double t2 = w3.y - w1.y;

  		double r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r, 0);

		tan1[triangles[i]] += sdir;
		tan1[triangles[i + 1]] += sdir;
		tan1[triangles[i + 2]] += sdir;
		
		tan2[triangles[i]] += tdir;
		tan2[triangles[i + 1]] += tdir;
		tan2[triangles[i + 2]] += tdir;

	}

	for(uint64_t i=0; i<_vertices.size(); i++) { 
		glm::vec3 n = normals[i];
		glm::vec3 t = glm::vec3(tan1[i]);

		tangents[i] = glm::vec4(glm::normalize((t - n * glm::dot(n, t))), 1);
		
		tangents[i].w = (glm::dot(glm::cross(n, t), glm::vec3(tan2[i])) < 0.0F) ? -1.0F : 1.0F;
	}
}


void MeshFilterComponent::LoadFromBuffers(std::vector<glm::vec3> _vertex,
        std::vector<glm::vec3> _normals,
        std::vector<glm::vec2> _uv,
        std::vector<glm::vec4> _colors,
        std::vector<int> _triangles, 
        bool calculateTangents) {
    this->triangles = _triangles;

	std::vector<glm::vec4> tangents(_normals.size(), glm::vec4(0, 0, 0, 0));
	if(calculateTangents) CalculateTangents(tangents, _vertex, _normals, _uv, _triangles);
	vertices.resize(0);
    for(int i=0; i<_vertex.size(); i++) {
        Vertex vertex = {
            _vertex[i],
            _normals[i],
            _uv[i],
            _colors[i].r,
            _colors[i].g,
            _colors[i].b,
            _colors[i].a,
			tangents[i]
        };
        vertices.push_back(vertex);
		
    }
}

void MeshFilterComponent::InitBuffers() {
	QOpenGLContext* context = QOpenGLContext::currentContext();
	if (context == nullptr) { std::cout << "could not get opengl context"; }

	QOpenGLFunctions_3_3_Core* ogl = context->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (ogl == nullptr) { std::cout << "could not get opengl functions"; }

	ogl->glGenVertexArrays(1, &vertexArrayObject);
	ogl->glGenBuffers(1, &vertexBuffer);
	ogl->glGenBuffers(1, &elementBuffer);
	
	//Bind VAO
	ogl->glBindVertexArray(vertexArrayObject);
	
	//bind buffers
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	
	ogl->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//set vertex attributes
	ogl->glEnableVertexAttribArray(0);
	ogl->glEnableVertexAttribArray(1);
	ogl->glEnableVertexAttribArray(2);
	ogl->glEnableVertexAttribArray(3);
	ogl->glEnableVertexAttribArray(4);
	ogl->glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)0));
	ogl->glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)12));
	ogl->glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)24));
	ogl->glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)((uintptr_t)32));
	ogl->glVertexAttribPointer(4, 4, GL_FLOAT, true, sizeof(Vertex), (void*)((uintptr_t)36));
	ogl->glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Init matrices buffer
	ogl->glGenBuffers(1, &matricesBuffer);
	ogl->glBindBuffer(GL_ARRAY_BUFFER, matricesBuffer);

  	GLsizei vec4Size = sizeof(glm::vec4);
    ogl->glEnableVertexAttribArray(5); 
    ogl->glEnableVertexAttribArray(6); 
    ogl->glEnableVertexAttribArray(7); 
    ogl->glEnableVertexAttribArray(8);
    ogl->glEnableVertexAttribArray(9);
    ogl->glEnableVertexAttribArray(10);

    ogl->glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)0);
    ogl->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)(vec4Size));
    ogl->glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)(2 * vec4Size));
    ogl->glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)(3 * vec4Size));
    ogl->glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)(4 * vec4Size));
    ogl->glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceAttribute), (void*)(5 * vec4Size));

    ogl->glVertexAttribDivisor(5, 1);
    ogl->glVertexAttribDivisor(6, 1);
    ogl->glVertexAttribDivisor(7, 1);
    ogl->glVertexAttribDivisor(8, 1);
    ogl->glVertexAttribDivisor(9, 1);
    ogl->glVertexAttribDivisor(10, 1);

	ogl->glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind VAO
	ogl->glBindVertexArray(0);
	//Unbind array and element buffers
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	inited = true; 
}

void MeshFilterComponent::DestroyBuffers() { 
	GETGL
	if(inited)
	{
		ogl->glDeleteBuffers(1, &vertexBuffer);
		ogl->glDeleteBuffers(1, &elementBuffer);
		ogl->glDeleteBuffers(1, &matricesBuffer);
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

	instanceAttributes = std::vector<InstanceAttribute>(numInstances);
	for(int i=0; i<numInstances; i++) {
		instanceAttributes[i].modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, (float)i, 0));
		instanceAttributes[i].additionalData1 = glm::vec4(0, 0, 0, 0);
		instanceAttributes[i].additionalData2 = glm::vec4(0, 0, 0, 0);
	}
	ogl->glBindBuffer(GL_ARRAY_BUFFER, matricesBuffer);
	ogl->glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(InstanceAttribute), (uint8_t*)&instanceAttributes[0], GL_STREAM_DRAW);
	
	//Unbind array and element buffers
	ogl->glBindVertexArray(0);
	ogl->glBindBuffer(GL_ARRAY_BUFFER, 0);
	ogl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    
}

void MeshFilterComponent::SetInstanceAttributes(std::vector<InstanceAttribute> instanceAttributes) {
	GETGL
	//Bind VAO
	ogl->glBindVertexArray(vertexArrayObject);
	
	//Bind buffers
	ogl->glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	ogl->glBindBuffer(GL_ARRAY_BUFFER, matricesBuffer);
	ogl->glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(InstanceAttribute), (uint8_t*)&instanceAttributes[0], GL_STREAM_DRAW);
	
	//Unbind array and element buffers
	ogl->glBindVertexArray(0);
	ogl->glBindBuffer(GL_ARRAY_BUFFER, 0);

}

std::vector<int> MeshFilterComponent::GetTriangles() {
	return triangles;
}


QJsonObject MeshFilterComponent::ToJSON() {
	QJsonObject json;
	json["type"] = "MeshFilter";
	json["drawingMode"] = (int)drawingMode;
	json["polygonMode"] = (int)polygonMode;
	json["primitiveSize"] = primitiveSize;
	json["meshType"] = meshType;
	json["modelPath"] = QString::fromStdString(modelpath);
	json["renderInstanced"] = renderInstanced;
	json["numInstances"] = numInstances;
	return json;
}	

void MeshFilterComponent::FromJSON(QJsonObject json, Object3D* obj) {
	std::vector<glm::vec3> vertex;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec4> colors;
	std::vector<int> triangles;

	MeshFilterComponent* mesh = new MeshFilterComponent(obj);
	mesh->drawingMode = json["drawingMode"].toInt();
	mesh->polygonMode = json["polygonMode"].toInt();
	mesh->primitiveSize = json["primitiveSize"].toInt();
	mesh->modelpath = json["modelPath"].toString().toStdString();
	mesh->renderInstanced = json["renderInstanced"].toBool();
	mesh->numInstances = json["numInstances"].toInt();

	mesh->meshType = (PRIMITIVE_MESH) json["meshType"].toInt();
	enum PRIMITIVE_MESH {MODEL_MESH=0, CUBE_MESH=1, SPHERE_MESH=2, CIRCLE_MESH=3, CONE_MESH=4, QUAD_MESH=5, MESH_PRIMITIVE_ENUM_SIZE=6};
	if(mesh->meshType == PRIMITIVE_MESH::MODEL_MESH) {
		Util::FileIO::LoadModel(mesh->modelpath, &vertex, &normals, &uv, &colors, &triangles);
	} else if(mesh->meshType == PRIMITIVE_MESH::CUBE_MESH) {
        GetCubeBuffers(&vertex, &normals, &uv, &colors, &triangles);
	} else if(mesh->meshType == PRIMITIVE_MESH::SPHERE_MESH) {
        GetSphereBuffers(&vertex, &normals, &uv, &colors, &triangles);
	} else if(mesh->meshType == PRIMITIVE_MESH::CIRCLE_MESH) {
        GetCircleBuffers(&vertex, &normals, &uv, &colors, &triangles);
	} else if(mesh->meshType == PRIMITIVE_MESH::CONE_MESH) {
        GetConeBuffers(&vertex, &normals, &uv, &colors, &triangles);
	} else if(mesh->meshType == PRIMITIVE_MESH::QUAD_MESH) {
        GetQuadBuffers(&vertex, &normals, &uv, &colors, &triangles);
	}  
	
	mesh->LoadFromBuffers( vertex, normals, uv, colors, triangles, true);
	
	obj->AddComponent(mesh);
}



}
}