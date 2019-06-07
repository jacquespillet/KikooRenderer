#include "StandardShaders.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{

void StandardShaders::Compile()
{	
unlitMeshShader.vertSrc= R"(
//attribs
#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
//transforms
uniform mat4 modelViewProjectionMatrix;
uniform vec4 materialColor; 
//outputs
out vec4 fragmentColor;  
out float fragmentDepth;
//main
void main()
{
	//compute outputs
	fragmentColor = color;
	vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
	gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, 1.0f);
}
)";

unlitMeshShader.fragSrc = R"(
//inputs
#version 440
in vec4 fragmentColor; 
//uniforms
//output
layout(location = 0) out vec4 outputColor; 
//main
void main()
{
	outputColor = fragmentColor;
}
)";

std::cout << "StandardShaders: Compiling unlitMeshShader" << std::endl; 
unlitMeshShader.Compile();

}	
}
}