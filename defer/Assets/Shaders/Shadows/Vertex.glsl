#version 450 core

layout( location=0 ) in vec4 Position;
layout( location=1 ) in vec4 Normal;
layout( location=2 ) in vec4 Tangent;
layout( location=3 ) in vec2 TexCoord;

uniform mat4 uLightMatrix;
uniform mat4 uModel;

void main() {
    gl_Position = uLightMatrix * uModel * Position;
}
