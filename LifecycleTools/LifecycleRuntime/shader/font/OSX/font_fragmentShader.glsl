#version 120

uniform vec4 unif_modulateColor;
uniform sampler2D unif_texture;

varying vec2 texCoord;
varying vec4 color;
 
void main(void) 
{
	vec4 temp_color = color * unif_modulateColor * texture2D(unif_texture, texCoord);
    gl_FragColor = temp_color;
    
    return;
}