#version 150

/**
-------------- bloom vertex shader -------------
    author: Richman Stewart
    simple vertex shader that sets the position
    to the specified matrix and position while
    passing the vertex colour and tex coords
    to the fragment shader
**/

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

in vec4 position;
in vec4 color;
in vec4 normal;
in vec2 texcoord;

out vec4 v_colour;
out vec2 v_texcoord;
out vec2 pos;

void main() {
	pos = position.xy;
	v_texcoord = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}
