#version 100

uniform mediump vec4 unif_modulateColor;
uniform mediump vec4 unif_interpolateColor;
uniform sampler2D unif_texture;

varying mediump vec2 texCoord;
 
void main(void) 
{
	mediump vec4 color = unif_modulateColor * texture2D(unif_texture, texCoord);
    gl_FragColor.xyz = (unif_interpolateColor.xyz * unif_interpolateColor.w) + (color.xyz * (1.0 - unif_interpolateColor.w));
    gl_FragColor.w = color.w;
    
    return;
}