#version 450 core

layout( location=0 ) in vec4 Position;
layout( location=1 ) in vec4 Normal;
layout( location=2 ) in vec4 Tangent;
layout( location=3 ) in vec2 TexCoord;

out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 uProjection;
uniform mat4 uView;
//uniform mat4 uModel;

void main() {
    vPosition = uView * Position;
    vNormal = normalize( uView * Position );
    vTexCoord = TexCoord;
    gl_Position = uProjection * uView * Position;
}
