#version 150

uniform sampler2DRect tex0;
uniform int imgWidth,imgHeight;
uniform float rand;
uniform int flags;
uniform vec2 blur_vec;
uniform float val1,val2,val3,val4;
int flgs;
float pix_w,pix_h;
in vec2 pos;

out vec4 outColor;

void main (void)
{
    //pix_w = 1.0 / float(imgWidth);
    //pix_h = 1.0 / float(imgHeight);
    flgs = flags;
    
	float slit_h = val3;
	
    vec2 texCoord = vec2(3.0+floor(pos.x/slit_h)*slit_h ,pos.y);
    vec4 col = texture2DRect(tex0,texCoord);
	
    outColor = col.rgba;
}
