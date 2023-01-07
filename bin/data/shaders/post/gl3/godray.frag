#version 150

uniform sampler2DRect tex0;

in vec2 pos;
out vec4 outColor;

void main (void)
{
    outColor = vec4(0.5);
}
