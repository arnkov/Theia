#version 150

uniform sampler2DRect tex0;

//uniform vec2 enabled;
uniform int size;
uniform float separation;
uniform float threshold;
uniform float amount;
uniform vec2 resolution;

in vec2 pos;
in vec2 v_texcoord;

out vec4 fragColor;

void main() {

	//vec2 texSize = textureSize(tex0);
	vec2 texSize = vec2(1.0); 

	vec2 texCoord = vec2(pos.x, pos.y);
	vec4 col = texture2DRect(tex0, texCoord);

	vec4 result = vec4(0.0);
	vec4 color  = vec4(0.0);

	float value = 0.0;
	float count = 0.0;

	for (int i = -size; i <= size; ++i) {
		for (int j = -size; j <= size; ++j) {
			color =
			texture
			( tex0
			, (vec2(i, j) * separation*2.5 + pos.xy)
            / texSize
			);

	value = max(color.r, max(color.g, color.b));
	if (value < threshold) { color = vec4(0.0); }

	result += color;
	count  += 1.0;
    }
  }

  result /= count;

	vec4 bloomColor = mix(vec4(0.0), result, amount);
	fragColor = col + 0.5*bloomColor;

}
