#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;
out vec4 v_position;

void main(void)
{
	v_position = position;
    gl_Position = modelViewProjectionMatrix * position;
}
