#version 150

uniform vec4 unif_modulateColor;
uniform vec4 unif_interpolateColor;
uniform sampler2D unif_texture;

in vec2 texCoord;

out vec4 frag_color;
 
void main(void) 
{
	vec4 color = unif_modulateColor * texture2D(unif_texture, texCoord);
    frag_color.xyz = (unif_interpolateColor.xyz * unif_interpolateColor.w) + (color.xyz * (1.0 - unif_interpolateColor.w));
    frag_color.w = color.w;
    
    return;
}