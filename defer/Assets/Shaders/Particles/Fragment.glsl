#version 450 core

in vec4 Color;
in vec4 Normal;

layout( location = 0 ) out vec3 particleAlbedo;
layout( location = 1 ) out vec4 particlePosition;
layout( location = 2 ) out vec3 particleNormal;

void main() {
    particleAlbedo = Color.xyz;
    particlePosition = vec4( 0.0, 0.0, 0.0, -2.0 );
    particleNormal = Normal.xyz;
}
