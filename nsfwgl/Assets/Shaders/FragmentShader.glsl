#version 430 core

in VS_OUT {
    vec3 pos;
    vec4 color;
} fs_in;

out vec4 color;

void main( void ) {
    color = fs_in.color;
}
