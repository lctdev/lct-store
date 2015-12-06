#version 100

uniform mat3 unif_worldTransform;
uniform mat4 unif_projectionTransform;

attribute vec3 attr_position;
attribute vec2 attr_texCoord;
attribute vec4 attr_color;

varying vec2 texCoord;
varying vec4 color;
 
void main(void) 
{
    vec3 position3 = vec3(attr_position.x, attr_position.y, 1.0);
    position3 = unif_worldTransform * position3;
    vec4 position4 = vec4(position3.x, position3.y, attr_position.z, 1.0);
    position4 = unif_projectionTransform * position4;
    gl_Position = position4;
    texCoord = attr_texCoord;
    color = attr_color;

    return;
}
