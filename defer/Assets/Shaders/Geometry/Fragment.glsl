#version 410 core

in VS_OUT {
    vec4 position;
    vec4 normal;
} fs_in;

layout( location = 0 ) out vec3 gpassAlbedo;
layout( location = 1 ) out vec3 gpassPosition;
layout( location = 2 ) out vec3 gpassNormal;

void main() {
    gpassAlbedo = vec3( 1 );
    gpassPosition = fs_in.Position.xyz;
    gpassNormal = fs_in.normal.xyz;
}
