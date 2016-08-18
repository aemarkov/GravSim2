#version 330

in vec3 in_Position;


out vec4 ex_Color;
 
void main(void) 
{
    gl_Position = vec4(in_Position.x*0.5, in_Position.y*0.5, in_Position.z*0.5, 1.0);
}