#version 150

uniform sampler2DRect tex0;
in vec2 pos;

out vec4 outColor;

void main()
{
	int step = 5;
	vec4 cols[25];
    vec2 texCoord = vec2(pos.x,pos.y);//vec2(min(max(0.0,pos.x),1.0),min(max(0.0,pos.y),1.0));

	for (int i = 0;i < step;i++){
		for (int j = 0;j < step;j++){
			cols[i*step+j] = texture2DRect(tex0,vec2((pos.x+float(j)-1.0), (pos.y+float(i)-1.0)));
			cols[i*step+j].r = (cols[i*step+j].r + cols[i*step+j].g + cols[i*step+j].b) / 2.0;
		}
	}
	float mn = 1.0 , mx = 0;
	for (int i = 0;i < step*step;i++){
		mn = min(cols[i].r,mn);
		mx = max(cols[i].r,mx);
	}
	float dst = (mx-mn);
	outColor.a = 1.0;
    outColor.rgb = vec3(dst,dst,dst);

}
