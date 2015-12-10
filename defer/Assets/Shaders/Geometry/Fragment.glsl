#version 450 core

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;

layout( location = 0 ) out vec3 gpassAlbedo;
layout( location = 1 ) out vec4 gpassPosition;
layout( location = 2 ) out vec3 gpassNormal;

uniform sampler2D uAlbedo;
uniform vec4 uColor = vec4( 1.0, 1.0, 1.0, 1.0 );
uniform int uUseTexture = 0;

void main() {
    if( uUseTexture > 0 ) {
        gpassAlbedo = texture( uAlbedo, vTexCoord ).xyz * uColor.xyz;
    } else {
        gpassAlbedo = uColor.xyz;
    }
    gpassPosition = vPosition;
    gpassNormal = vNormal.xyz;
}
