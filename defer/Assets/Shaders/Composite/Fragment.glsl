#version 450 core

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uAlbedoTexture;
uniform sampler2D uLightTexture;

void main() {
    vec3 light = texture( uLightTexture, vTexCoord ).rgb;
    vec3 albedo = texture( uAlbedoTexture, vTexCoord ).rgb;
    FragColor = vec4( albedo * light, 1 );
}
