 //attribs
#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
layout(location = 5) in mat4 instanceModelMatrix;
layout(location = 9) in vec4 instanceAdditionalData;

//transforms
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec4 albedo; 
uniform float materialInfluence;
uniform int hasCubemap;
uniform vec4 clipPlane;

//outputs
out vec4 fragmentColor;  
out vec2 fragmentUv;
out vec3 cubeTexCoords;

//main
void main()
{
    vec4 worldPos =  modelMatrix * vec4(position.x, position.y, position.z, 1.0f);
    gl_ClipDistance[0] = dot(worldPos, clipPlane);

    fragmentColor = materialInfluence * albedo + (1.0 - materialInfluence) * color;

    vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);

    fragmentUv = uv;
    cubeTexCoords = position.xyz;

    gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
    if(hasCubemap > 0) gl_Position = gl_Position.xyww; 
}