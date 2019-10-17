#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
layout(location = 4) in vec4 tangent;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;

uniform int flipNormals;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragUv;
out vec4 fragTangent;
out vec3 toCamTangentSpace; 
out vec3 toLightTangentSpace; 

void main()
{
    vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
    gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
    fragPos = (modelMatrix * vec4(position.x, position.y, position.z, 1.0f)).xyz;
    // fragNormal = (flipNormals > 0) ? -mat3(transpose(inverse(modelMatrix))) * normal  : mat3(transpose(inverse(modelMatrix))) * normal;
    fragNormal = (flipNormals > 0) ? -normal  : normal;
    fragUv = uv;
    fragTangent = tangent;
}