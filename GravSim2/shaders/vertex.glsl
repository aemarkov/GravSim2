#version 330

in vec3 Position;
in vec4 in_Color;
out vec4 Color;

uniform mat4 gWorld;
 
void main(void) 
{
    gl_Position =  gWorld * vec4(Position.x, Position.y, Position.z, 1.0);
	gl_PointSize = 1.0f;
	Color = in_Color;//vec4(clamp(Position, 0.0, 1.0), 1.0);
}