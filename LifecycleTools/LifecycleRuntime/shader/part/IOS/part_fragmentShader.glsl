#version 100

uniform sampler2D unif_texture;

varying mediump vec2 texCoord;
varying mediump vec4 color;
 
void main(void) 
{
	//vec4 temp_color = color * unif_modulateColor * texture2D(unif_texture, texCoord);
	mediump vec4 temp_color = color;
    gl_FragColor = temp_color;
    
    return;
}