#include "SelectedShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
Shader GetSelectedShader() {
    Shader selectedShader;
    selectedShader.vertSrc= R"(
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

    selectedShader.fragSrc = R"(
    #version 440
    layout(location = 0) out vec4 outputColor; 
    void main()
    {
        outputColor = vec4(1.0, 0.549, 0.0, 1.0);
    }
    )";

    return selectedShader;
}
}
}