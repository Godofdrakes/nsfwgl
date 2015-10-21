#version 450 core

layout( location = 0 ) in vec4 inPosition;

in vec2 vTexCoord;

void main() {
    gl_Position = inPosition;
}
