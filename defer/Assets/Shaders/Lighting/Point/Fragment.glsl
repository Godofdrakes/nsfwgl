#version 450 core

in vec2 vTexCoord;

out vec3 LightOutput;

struct PointLight {
    vec3 position;
    vec3 color;
};

uniform PointLight uPointLight;
uniform mat4 uCameraView = mat4( 1 );
uniform vec3 uCameraPosition = vec3( 0, 0, -5 );
uniform sampler2D uNormalTexture;
uniform sampler2D uPositionTexture;

void main() {
    vec3 normal = normalize( texture( uNormalTexture, vTexCoord ).xyz );
    vec3 position = texture( uPositionTexture, vTexCoord ).xyz;
    float specularLightPower = texture( uPositionTexture, vTexCoord ).w;

    // Diffuse Lighting
    vec3 lightDirection = ( uCameraView * vec4( uPointLight.position - position, 0 ) ).xyz;
    float diffuseLight = dot( normal, lightDirection ) / ( length(lightDirection) * length( normal ) );
    diffuseLight = clamp( diffuseLight, 0, 1 );
    vec3 diffuseColor = uDirectionalLight.color * diffuseLight;

    // Specular lighting
	vec3 viewPointDirection = normalize(uCameraPosition - position);
	vec3 halfVector = normalize(lightDirection + viewPointDirection);
	float specularLight = pow(max(dot(normal, halfVector), 0), specularLightPower);
	if (diffuseLight <= 0) { specularLight = 0; }
	vec3 specularColor = uDirectionalLight.color * specularLight;

    LightOutput = uAmbientLightColor + diffuseColor + specularColor;
}
