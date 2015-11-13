#version 450 core

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight uDirectionalLight;
uniform vec3 uAmbientLightColor = vec3( 0.25, 0.25, 0.25 );
//uniform float uSpecularLightPower = 0;
uniform mat4 uCameraView = mat4( 1 );
uniform vec3 uCameraPosition = vec3( 0, 0, -5 );

uniform sampler2D uNormalTexture;
uniform sampler2D uPositionTexture;

uniform bool uEnableShadows = false;
uniform sampler2D uShadowMap;
uniform float uShadowBias = 0.01;
uniform mat4 uLightMatrix;

void main() {
    vec3 normal = normalize( texture( uNormalTexture, vTexCoord ).xyz );
    vec3 position = texture( uPositionTexture, vTexCoord ).xyz;
    float specularLightPower = texture( uPositionTexture, vTexCoord ).w;
    vec4 vShadowCoord = uLightMatrix * inverse( uCameraView ) * vec4( position, 1 );

    // Diffuse Lighting
    vec3 lightDirection = ( uCameraView * vec4( uDirectionalLight.direction, 0 ) ).xyz;
    float diffuseLight = max( dot( normal, lightDirection ), 0 );
    if( uEnableShadows ) {
        if( texture( uShadowMap, vShadowCoord.xy ).r < vShadowCoord.z - uShadowBias ) { diffuseLight = 0; }
    }
    vec3 diffuseColor = uDirectionalLight.color * diffuseLight;

    // Specular lighting
	vec3 viewPointDirection = normalize(uCameraPosition - position);
	vec3 halfVector = normalize(lightDirection + viewPointDirection);
	float specularLight = pow(max(dot(normal, halfVector), 0), specularLightPower);
	if (diffuseLight <= 0) { specularLight = 0; }
	vec3 specularColor = uDirectionalLight.color * specularLight;

    LightOutput = uAmbientLightColor + diffuseColor + specularColor;
}
