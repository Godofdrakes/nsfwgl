#version 450 core

out float outFragDepth;

void main() {
    outFragDepth = gl_FragCoord.z;
}
