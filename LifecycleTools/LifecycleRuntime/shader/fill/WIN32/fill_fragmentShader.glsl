#version 150

uniform vec4 unif_modulateColor;

out vec4 frag_color;
 
void main(void) 
{
	vec4 color = unif_modulateColor;
    frag_color = color;
    
    return;
}