#version 450 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;

layout( location = 0 ) out vec3 gpassAlbedo;
layout( location = 1 ) out vec3 gpassPosition;
layout( location = 2 ) out vec3 gpassNormal;

uniform sampler2D uDiffuse;

void main() {
    //gpassAlbedo = vec3( 1 );
    gpassAlbedo = texture( uDiffuse, vTexCoord ).xyz;
    gpassPosition = vPosition.xyz;
    gpassNormal = vNormal.xyz;
}
