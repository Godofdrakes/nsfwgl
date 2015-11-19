#version 450 core

layout( location=0 ) in vec4 Position;
layout( location=1 ) in vec4 Normal;
layout( location=2 ) in vec4 Tangent;
layout( location=3 ) in vec2 TexCoord;

out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 uProjection = mat4( 1 );
uniform mat4 uView       = mat4( 1 );
uniform mat4 uModel      = mat4( 1 );
uniform float uSpecularLightPower = 0;
uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;

void main() {
    vPosition = uView * uModel * Position;
    vPosition.w = uSpecularLightPower;

    vec4 modelNormal = uModel * Normal;
    vec4 modelTangent = uModel * Tangent;
    vec4 binormal = vec4( cross( modelNormal.xyz, modelTangent.xyz ), 0 );
    mat4 tbn = mat4( modelNormal, binormal, modelNormal, vec4( 0 ) );

    vec4 normalSample = texture( uNormalMap, TexCoord ) * 2 - 1;
    vNormal = normalize( uView * tbn * normalSample );
    vTexCoord = TexCoord;
    gl_Position = uProjection * uView * uModel * Position;
}
