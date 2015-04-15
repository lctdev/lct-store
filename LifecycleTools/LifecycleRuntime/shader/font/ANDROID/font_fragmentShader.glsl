#version 100

uniform mediump vec4 unif_modulateColor;
uniform sampler2D unif_texture;

varying mediump vec2 texCoord;
varying mediump vec4 color;
 
void main(void) 
{
	mediump vec4 temp_color = color * unif_modulateColor * texture2D(unif_texture, texCoord);
    gl_FragColor = temp_color;
    
    return;
}