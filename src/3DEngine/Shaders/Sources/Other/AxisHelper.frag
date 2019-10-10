#version 440
layout(location = 0) out vec4 outputColor; 

in vec4 fragmentColor;  

void main()
{
    outputColor = fragmentColor;

}