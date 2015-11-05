#version 450 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;

layout( location = 0 ) out vec3 gpassAlbedo;
layout( location = 1 ) out vec4 gpassPosition;
layout( location = 2 ) out vec3 gpassNormal;

uniform sampler2D uAlbedo;

void main() {
    gpassAlbedo = texture( uAlbedo, vTexCoord ).xyz;
    gpassPosition = vPosition;
    gpassNormal = vNormal.xyz;
}
