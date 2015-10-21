#version 450 core

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inTexCoord;

out vec2 vTexCoord;

void main() {
    vTexCoord = inTexCoord;
    gl_Position = inPosition;
}
