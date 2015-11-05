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

void main() {
    vec3 normal = normalize( texture( uNormalTexture, vTexCoord ).xyz );
    vec3 position = texture( uPositionTexture, vTexCoord ).xyz;
    float specularLightPower = texture( uPositionTexture, vTexCoord ).w;

    // Diffuse Lighting
    vec3 lightDirection = ( uCameraView * vec4( uDirectionalLight.direction, 0 ) ).xyz;
    float diffuseLight = max( dot( normal, lightDirection ), 0 );
    vec3 diffuseColor = uDirectionalLight.color * diffuseLight;

    // Specular Lighting
    // Don't need to do this. As this is a GLOBAL directional light we pass in direction and not position, menaing this conversion from position to direction is not necessary.
    // vec3 lightVector = normalize( lightPosition - uCameraPosition );

    //vec3 specularColor = vec3( 0, 0, 0 );
    // If diffuseLight is 0 ( meaning the surface is facing AWAY from the light ) then Specular Lighting should not be calculated.
    //if( uSpecularLightPower != 0 && diffuseLight != 0 ) {
    //    vec3 viewVector = normalize( uCameraPosition - position );
    //    vec3 halfway = normalize( uDirectionalLight.direction + viewVector );
    //    float specularLight = pow( max( dot( normal, halfway ), 0 ), uSpecularLightPower );
    //    specularColor = uDirectionalLight.color * specularLight;
    //}

	vec3 viewPointDirection = normalize(uCameraPosition - position);
	vec3 halfVector = normalize(lightDirection + viewPointDirection);
	float specularLight = pow(max(dot(normal, halfVector), 0), specularLightPower);
	if (diffuseLight <= 0)
	{
		specularLight = 0;
	}
	vec3 specularColor = uDirectionalLight.color * specularLight;

    LightOutput = uAmbientLightColor + diffuseColor + specularColor;
}
