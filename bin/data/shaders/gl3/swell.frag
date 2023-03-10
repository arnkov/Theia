#version 150

uniform sampler2DRect tex0;
uniform int imgWidth,imgHeight;
uniform float rand;
uniform int flags;
uniform vec2 blur_vec;
uniform float timer;
int flgs;
float pix_w,pix_h;
in vec2 pos;

out vec4 outColor;

void main (void)
{
    pix_w = 1.0;
    pix_h = 1.0;
    flgs = flags;
    
    vec2 texCoord = vec2(pos.x,pos.y);
    vec4 col = texture2DRect(tex0,texCoord);
    vec4 col_s = texture2DRect(tex0,texCoord + vec2(sin(pos.y*0.03+timer*20.0)*(6.0+12.0*rand),0));
    
    col = col_s;
//    col.r = col.r * sin(rand);
//	col.g = col.g * sin(rand*1.34+pos.y);
//	col.b = col.b * cos(rand*1.56+pos.x*pos.y);
	
    outColor = col.rgba;
}
