#version 330
//#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

smooth out vec4 theColor;

void main()
{
    gl_Position = ftransform();
    theColor = color;
}