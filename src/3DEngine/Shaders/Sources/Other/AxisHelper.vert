 //attribs
#version 440
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
layout(location = 5) in mat4 instanceModelMatrix;
layout(location = 9) in vec4 instanceAdditionalData;

//transforms
uniform mat4 viewRotationMatrix;
uniform mat4 projectionMatrix;

//outputs
out vec4 fragmentColor;  

//main
void main()
{
    fragmentColor = vec4(color.xyz, 1);
    
    mat4 modifiedMVP = projectionMatrix *  viewRotationMatrix;

    vec4 finalPosition = modifiedMVP * vec4(position.x, position.y, position.z, 1.0f);
    finalPosition.w = 1;

    finalPosition.x *= 0.05;
    finalPosition.y *= 0.05;
    finalPosition.z *= 0.05;
    
    finalPosition.x -= 0.8;
    finalPosition.y -= 0.8;

    gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
}