#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in  vec4 Color;
 
void main(void)
{
    gl_FragColor = Color;
}