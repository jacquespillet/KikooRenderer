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
uniform vec4 albedo; 
uniform float materialInfluence;
//outputs
out vec4 fragmentColor;  
out float fragmentDepth;
//main
void main()
{
	//compute outputs
	fragmentColor = materialInfluence * albedo + (1.0 - materialInfluence) * color;
	vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
	gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
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

selectedObjectShader.vertSrc= R"(
//attribs
#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
//transforms
uniform mat4 modelViewProjectionMatrix;
void main()
{
	vec3 scaledPosition = position * 1.05;
	vec4 finalPosition = modelViewProjectionMatrix * vec4(scaledPosition.x, scaledPosition.y, scaledPosition.z, 1.0f);
	gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
}
)";

selectedObjectShader.fragSrc = R"(
#version 440
layout(location = 0) out vec4 outputColor; 
void main()
{
	outputColor = vec4(1.0, 0.549, 0.0, 1.0);
}
)";

std::cout << "StandardShaders: Compiling selectedObjectShader" << std::endl; 
selectedObjectShader.Compile();


}	
}
}