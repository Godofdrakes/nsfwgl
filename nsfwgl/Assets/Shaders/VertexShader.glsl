#version 430 core

layout( location = 0 ) in vec3 inPosition;
layout( location = 1 ) in vec4 inColor;

out VS_OUT {
    vec3 pos;
    vec4 color;
} vs_out;

uniform mat4 Model;
uniform

void main( void ) {
    vs_out.pos = inPosition;
    vs_out.color = inColor;
    gl_Position = inPosition;
}
