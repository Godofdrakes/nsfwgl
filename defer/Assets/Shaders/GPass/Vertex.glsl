#version 410 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inNormal;

out VS_OUT {
    vec4 position;
    vec4 normal;
} vs_out;

uniform mat4 uView;
uniform mat4 uProjectionView;

void main() {
    vs_out.position = uView * inPosition;
    vs_out.normal = normalize( uView * inNormal );
    gl_Position = uProjectionView * inPosition;
}
