#version 150

uniform vec4 unif_modulateColor;
uniform sampler2D unif_texture;

in vec2 texCoord;
in vec4 color;

out vec4 frag_color;
 
void main(void) 
{
	vec4 temp_color = color * unif_modulateColor * texture2D(unif_texture, texCoord);
    frag_color = temp_color;
    
    return;
}