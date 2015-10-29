#version 450 core

in vec2 vTexCoord;

out vec3 LightOutput;

uniform vec3 uLightDirection;
uniform vec3 uLightDiffuse;
uniform sampler2D uNormalTexture;
uniform sampler2D uPositionTexture;

void main() {
    vec3 normal = normalize( texture( uNormalTexture, vTexCoord ).xyz );
    vec3 position = texture( uPositionTexture, vTexCoord ).xyz;
    float d = max( 0, dot( normal, uLightDirection ) );
    LightOutput = uLightDiffuse * d;
}
