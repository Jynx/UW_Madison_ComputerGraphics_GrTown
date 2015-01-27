#version 330
 
layout (std140) uniform Matrices {
    mat4 pvm;
} ;
uniform bool day;
in vec4 position;
 
void main()
{
    gl_Position = ftransform(); ;
}