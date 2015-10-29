#version 450 core

in vec2 vTexCoord;

out vec3 LightOutput;

uniform sampler2D uNormalTexture;
uniform sampler2D uPositionTexture;

uniform vec3 uLightDirection = vec3( 0, 1, -1 );
uniform vec3 uLightDiffuse = vec3( 1, 1, 1 );

uniform vec3 uAmbientColor = vec3( 1, 1, 1 );

uniform vec3 uCameraPosition = vec3( 0, 0, -5 );

uniform float uSpecPower = 0;

void main() {
    vec3 normal = normalize( texture( uNormalTexture, vTexCoord ).xyz );
    vec3 position = texture( uPositionTexture, vTexCoord ).xyz;
    float d = max( 0, dot( normal, uLightDirection ) );
    LightOutput = uLightDiffuse * d;
}
