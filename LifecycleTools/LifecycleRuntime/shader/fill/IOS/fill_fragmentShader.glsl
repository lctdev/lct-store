#version 100

uniform mediump vec4 unif_modulateColor;
 
void main(void) 
{
	mediump vec4 color = unif_modulateColor;
    gl_FragColor = color;
    
    return;
}