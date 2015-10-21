#version 450 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec4 inNormal;

out vec4 vPosition;
out vec4 vNormal;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
    vPosition = uView * inPosition;
    vNormal = normalize( uView * inNormal );
    gl_Position = uProjection * uView * inPosition;
}
