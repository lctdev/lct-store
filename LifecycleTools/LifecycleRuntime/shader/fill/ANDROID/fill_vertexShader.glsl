#version 100

uniform vec4 unif_placement;
uniform mat3 unif_worldTransform;
uniform mat4 unif_projectionTransform;

attribute vec3 attr_position;

varying vec2 texCoord;
 
void main(void) 
{
    vec3 position3 = vec3(attr_position.x, attr_position.y, 1.0);
    position3.xy *= unif_placement.xy;
    position3.xy += unif_placement.zw;
    position3 = unif_worldTransform * position3;
    vec4 position4 = vec4(position3.x, position3.y, attr_position.z, 1.0);
    position4 = unif_projectionTransform * position4;
    gl_Position = position4;

    return;
}
