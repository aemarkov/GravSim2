#version 330
// It was expressed that some drivers required this next line to function properly
precision highp float;
 
in float Color;
 
vec4 packFloatToVec4i(int value)
{	
   //value = 0xFFFF;

  float r = (value & 0xFF) / 255.0;
  float g = (value & 0xFF00) / 255.0;
  float b = (value & 0xFF0000) / 255.0;
  float a = (value & 0xFF000000) / 255.0;

  return vec4(r,g,b,1);
}

void main(void)
{
    gl_FragColor = packFloatToVec4i(int(Color));
}

