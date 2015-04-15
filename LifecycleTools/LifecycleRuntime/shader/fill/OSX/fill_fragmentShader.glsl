#version 120

uniform vec4 unif_modulateColor;
 
void main(void) 
{
	vec4 color = unif_modulateColor;
    gl_FragColor = color;
    
    return;
}