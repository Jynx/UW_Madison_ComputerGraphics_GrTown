#version 330
//#extension GL_ARB_explicit_uniform_location : require

smooth in vec4 theColor;

out vec4 outputColor;

void main()
{
    outputColor = vec3(1.0,1.0,1.0);
}