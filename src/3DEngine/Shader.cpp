#include "Shader.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}


namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	

Shader::Shader()
{
	compiled = false; 
}

	
void Shader::Compile()
{
	GETGL
	bool hasGeometry=geometrySrc.size()>0;

	//make array to pointer for source code (needed for opengl )
	const char* vsrc[1];
	const char* fsrc[1];
	const char* gsrc[1];
	vsrc[0] = vertSrc.c_str();
	fsrc[0] = fragSrc.c_str();
	if(hasGeometry)gsrc[0] = geometrySrc.c_str();
	
	//compile vertex and fragment shaders from source
	vertexShaderObject = ogl->glCreateShader(GL_VERTEX_SHADER);
	ogl->glShaderSource(vertexShaderObject, 1, vsrc, NULL);
	ogl->glCompileShader(vertexShaderObject);
	fragmentShaderObject = ogl->glCreateShader(GL_FRAGMENT_SHADER);
	ogl->glShaderSource(fragmentShaderObject, 1, fsrc, NULL);
	ogl->glCompileShader(fragmentShaderObject);
	if(hasGeometry) {
		geometryShaderObject = ogl->glCreateShader(GL_GEOMETRY_SHADER);
		ogl->glShaderSource(geometryShaderObject, 1, gsrc, NULL);
		ogl->glCompileShader(geometryShaderObject); 
	}
	std::cout << "Shader:Linking shader:" << std::endl;
	
	//link vertex and fragment shader to create shader program object
	programShaderObject = ogl->glCreateProgram();
	ogl->glAttachShader(programShaderObject, vertexShaderObject);
	ogl->glAttachShader(programShaderObject, fragmentShaderObject);
	if(hasGeometry) ogl->glAttachShader(programShaderObject, geometryShaderObject);
	ogl->glLinkProgram(programShaderObject);
	std::cout << "Shader:checking shader status:" << std::endl;
	
	//Check status of shader and log any compile time errors
	int linkStatus;
	ogl->glGetProgramiv(programShaderObject, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) 
	{
		char log[5000];
		int logLen; 
		ogl->glGetProgramInfoLog(programShaderObject, 5000, &logLen, log);
		std::cerr << "Shader:Could not link program: " << std::endl;
		std::cerr << log << std::endl;
		ogl->glGetShaderInfoLog(vertexShaderObject, 5000, &logLen, log);
		std::cerr << "vertex shader log:\n" << log << std::endl;
		ogl->glGetShaderInfoLog(fragmentShaderObject, 5000, &logLen, log);
		std::cerr << "fragment shader log:\n" << log << std::endl;
		ogl->glDeleteProgram(programShaderObject);
		programShaderObject = 0;
	}
	else
	{
		std::cout << "Shader:compile success " << std::endl;
		compiled = true; 
	}
}

void Shader::SetId(int id) { 
	this->identifier = id; 
}

Shader& Shader::operator=(const Shader& other)
{
	vertSrc = other.vertSrc;
	fragSrc = other.fragSrc;
	geometrySrc = other.geometrySrc;
	isLit = other.isLit;
	isDepthPass = other.isDepthPass;
	name = other.name;
	SetId(other.identifier);
	
	if(other.shouldRecompile) {
		Compile();	
	} else {
		programShaderObject = other.programShaderObject;
	}
    return *this;
}

int Shader::GetId() { return this->identifier; }


}
}