#include "StandardShaders.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{

void StandardShaders::Compile()
{
unlitMeshShader.SetId(SHADER_IDS::UNLIT);	
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

selectedObjectShader.SetId(SHADER_IDS::SELECTED);
selectedObjectShader.vertSrc= R"(
#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

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

gouraudShader.SetId(SHADER_IDS::GOURAUD);
gouraudShader.vertSrc= R"(
#version 440

struct LightSource
{
        int type;
        vec3 position;
        vec3 attenuation;
        vec3 direction;
        vec4 color;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

//transforms
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec4 albedo; 
uniform vec3 cameraPos;

uniform LightSource lights[4];
uniform int numLights;
uniform sampler2D albedoTexture;

out vec4 fragColor;
out vec2 fragUv;
void main()
{
	vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
	vec4 worldPosition = modelMatrix * vec4(position.x, position.y, position.z, 1.0f);
	vec3 toCam = cameraPos - worldPosition.xyz;

	vec4 textureAlbedo = albedo * texture(albedoTexture, uv);

	vec4 finalColor = vec4(0, 0, 0, 1);
	finalColor.rgb += 0.5 * textureAlbedo.rgb;
	for(int i=0; i<numLights; i++) {

		float attenuation = 1;
		vec3 lightDirection = normalize(lights[i].direction);
		if(lights[i].type == 1) { //Point light
			float distance = distance(worldPosition.xyz, lights[i].position);
			attenuation = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
			lightDirection = normalize(worldPosition.xyz - lights[i].position);
		}
		if(lights[i].type == 2) { //Spot light
			lightDirection = normalize(worldPosition.xyz - lights[i].position);
			float distance = distance(worldPosition.xyz, lights[i].position);
			float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
			attenuation = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
		}
		
		vec3 toLight = -lightDirection;

		vec4 diffuse = textureAlbedo * lights[i].color * max(dot(normal, toLight), 0);
		diffuse.rgb *= 0.5;

		// Specular
		vec3 halfwayVec = normalize(toLight + toCam);
		vec4 specular = textureAlbedo * lights[i].color * pow(max(dot(normal, halfwayVec),0), 32);

		finalColor.rgb +=  attenuation * (diffuse + specular ).rgb;
	}
	fragColor = finalColor;
	fragUv = uv;
	gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
}
)";

gouraudShader.fragSrc = R"(
#version 440
layout(location = 0) out vec4 outputColor; 

in vec4 fragColor;
in vec2 fragUv;


void main()
{
	outputColor = fragColor;
}
)";

std::cout << "StandardShaders: Compiling gouraudShader" << std::endl; 
gouraudShader.Compile();

blinnPhongShader.SetId(SHADER_IDS::BLINNPHONG);
blinnPhongShader.vertSrc= R"(
#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragToCam;

void main()
{
	vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
	gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
	fragPos = (modelMatrix * vec4(position.x, position.y, position.z, 1.0f)).xyz;
	fragNormal = normal;
	fragToCam = normalize(cameraPos - fragPos.xyz);
}
)";

blinnPhongShader.fragSrc = R"(
#version 440

struct LightSource
{
        int type;
        vec3 position;
        vec3 attenuation;
        vec3 direction;
        vec4 color;
};

layout(location = 0) out vec4 outputColor; 
uniform LightSource lights[4];
uniform vec4 albedo; 
uniform int numLights; 

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragToCam;

void main()
{
	vec4 finalColor = vec4(0, 0, 0, 1);
	finalColor.rgb += 0.2 * albedo.rgb;

	for(int i=0; i<numLights; i++) {
		float attenuation = 1;
		vec3 lightDirection = normalize(lights[i].direction);
		
		if(lights[i].type == 1) { //Point light
			float distance = distance(fragPos.xyz, lights[i].position);
			attenuation = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
			lightDirection = normalize(fragPos.xyz - lights[i].position);
		}
		if(lights[i].type == 2) { //Spot light
			lightDirection = normalize(fragPos.xyz - lights[i].position);
			float distance = distance(fragPos.xyz, lights[i].position);
			float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
			attenuation = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
		}
		
		vec3 toLight = -lightDirection;

		vec4 diffuse = albedo * lights[i].color * max(dot(normalize(fragNormal), toLight), 0);
		diffuse.rgb *= 0.8;

		// Specular
		vec3 halfwayVec = normalize(toLight + fragToCam);
		vec4 specular = albedo * lights[i].color * pow(max(dot(normalize(fragNormal), halfwayVec),0), 32);

		finalColor.rgb +=  attenuation * (diffuse + specular ).rgb;
	}
	outputColor = finalColor;
}
)";

std::cout << "StandardShaders: Compiling blinnPhongShader" << std::endl; 
blinnPhongShader.Compile();


PBRShader.SetId(SHADER_IDS::PBR);
PBRShader.vertSrc= R"(
#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;

out vec3 fragPos;
out vec3 fragNormal;
out vec3 fragToCam;

void main()
{
	vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
	gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
	fragPos = (modelMatrix * vec4(position.x, position.y, position.z, 1.0f)).xyz;
	fragNormal = normal;
	fragToCam = normalize(cameraPos - fragPos.xyz);
}
)";

PBRShader.fragSrc = R"(
#version 440

struct LightSource
{
        int type;
        vec3 position;
        vec3 attenuation;
        vec3 direction;
        vec4 color;
};

float Fresnel(vec3 toLight, vec3 toCamera) {
	vec3 halfwayVec = normalize(toLight + toCamera);
	float dotLH =  max(dot(toLight, halfwayVec), 0);
	float refractionInx = (1.0 + sqrt(0.8)) / (1.0 - sqrt(0.8));

	float g = sqrt(refractionInx * refractionInx - 1.0 + pow(dotLH, 2.0)); 

	float firstTerm = 0.5 * (pow(g - dotLH, 2.0) / pow(g + dotLH, 2.0));
	float secondTermNumerator = pow(dotLH * (g + dotLH) - 1, 2.0); 
	float secondTermDenominator = pow(dotLH * (g - dotLH) + 1, 2.0);

	float result = firstTerm * (secondTermNumerator / secondTermDenominator + 1.0); 

	return result;
}


float MicrofacetsDistrib(vec3 toLight, vec3 toCamera, vec3 normal, float roughness) {
	vec3 halfwayVec = normalize(toLight + toCamera);
	float dotNH = max(dot(normal, halfwayVec), 0);

	float result = (1 / (4 * roughness * roughness * pow(dotNH, 4))) * exp( (dotNH * dotNH -1) / (roughness * roughness * dotNH * dotNH ));
	return result;
}

float GeometricalAttenuation(vec3 toLight, vec3 toCamera, vec3 normal) {
	vec3 halfwayVec = normalize(toLight + toCamera);
	float dotNH = dot(normal, halfwayVec);
	float dotNV = dot(normal, toCamera);
	float dotNL = dot(normal, toLight);
	float dotLH = dot(toLight, halfwayVec);
	
	
	float G1 = (2 * dotNH * dotNV) / dotLH;
	float G2 = (2 * dotNH * dotNL) / dotLH;

	return min (1, min(G1, G2));
}

layout(location = 0) out vec4 outputColor; 

uniform LightSource lights[4];
uniform int numLights; 
uniform vec4 albedo; 
uniform float roughness;
uniform float specularFrac;

in vec3 fragPos;
in vec3 fragNormal;
in vec3 fragToCam;

void main()
{
	// float roughness = 0.5;
	// float specularFrac = 0.5;
	float diffuseFrac = 1 - specularFrac;

	vec4 finalColor = vec4(0, 0, 0, 1);
	for(int i=0; i<numLights; i++) {
		vec3 lightDirection = normalize(lights[i].direction);
		float attenuation = 1;
		if(lights[i].type == 1) { //Point light
			float distance = distance(fragPos.xyz, lights[i].position);
			attenuation = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
			lightDirection = normalize(fragPos.xyz - lights[i].position);
		}
		if(lights[i].type == 2) { //Spot light
			lightDirection = normalize(fragPos.xyz - lights[i].position);
			float distance = distance(fragPos.xyz, lights[i].position);
			float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
			attenuation = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
		}
		vec3 toLight = -lightDirection;

		float fresnelFactor = Fresnel(toLight, fragToCam);
		float microfacetsDistribution = MicrofacetsDistrib(toLight, fragToCam, fragNormal, roughness);
		float geometricalAttenuation = GeometricalAttenuation(toLight, fragToCam, fragNormal);
		float finalFactor = fresnelFactor * ( (microfacetsDistribution * geometricalAttenuation) / (3.14 * dot(fragNormal, fragToCam) * dot(fragNormal, toLight) ) ); 
		
		float diffuseFactor = diffuseFrac * max(dot(normalize(fragNormal), toLight), 0);
		finalColor.rgb += attenuation * (diffuseFactor * lights[i].color.rgb * albedo.rgb +  specularFrac * lights[i].color.rgb * albedo.rgb * finalFactor);
	}



	outputColor = finalColor;
}
)";

std::cout << "StandardShaders: Compiling PBRShader" << std::endl; 
PBRShader.Compile();


}	
}
}