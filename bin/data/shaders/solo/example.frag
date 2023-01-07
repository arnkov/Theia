//declare the GLSL version according to the openGL version Theia is using
#version 150
//this shader is meant to be audio reactive, so make shure, Theia is listening.
//uniforms passed by Theia
uniform vec2 u_resolution;
uniform float u_time;
uniform vec3 u_beat;
uniform float u_rms;

//passed by vertex shader
in vec4 v_position;

//result
out vec4 outColor;

void main () {
	//normalize the uv coordinates. usually 0, 0, is at the bottom left.
	vec2 uv = 2.0 * (gl_FragCoord.xy / u_resolution) -1.0;
	//declare a color value.
	float col = 0.0;
	//shift the uvs an colours around thirteen times
	//feel free to try, what happens, if you change uv.x to uv.y in the sin()function
	for (float i=1.0; i<=13.0; i++){
		uv.x += sin( i * 20.0 + u_rms*0.8 - u_time *8.0 + uv.x * 2.5) * (u_beat.x/2.0 + 0.1);
		col += abs(0.966/uv.x) * (u_rms/15.0);
		uv.y += sin( i * 20 + u_rms - u_time *2.0 + uv.y * 2.5) * (u_beat.y/2.0 + 0.1);
		col += abs(0.966/uv.y) * (u_rms/12.0);
	}
	//okay, let's fill the rest of the screen
	float col2 = abs(uv.x * (u_beat.y -col));
	col2 *= abs(uv.x * cos(u_time*5.0));
	col2 -= col;
	float col3 = u_beat.y*0.5;
	col3 -= col;
	//combine all of it into the rgba outColor
	outColor = vec4(col, col3, col2, 1.0);

}
