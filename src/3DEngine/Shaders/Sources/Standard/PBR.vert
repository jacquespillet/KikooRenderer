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