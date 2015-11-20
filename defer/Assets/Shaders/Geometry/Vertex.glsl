#version 450 core

layout( location=0 ) in vec4 Position;
layout( location=1 ) in vec4 Normal;
layout( location=2 ) in vec4 Tangent;
layout( location=3 ) in vec2 TexCoord;

out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 uProjection          = mat4( 1 );
uniform mat4 uView                = mat4( 1 );
uniform mat4 uModel               = mat4( 1 );
uniform float uSpecularLightPower = 0;

uniform sampler2D uNormalMap;
uniform sampler2D uSpecularMap;

void main() {
    vPosition = uView * uModel * Position;
    vPosition.w = uSpecularLightPower;

    vec4 normalSample = vec4( normalize( texture( uNormalMap, TexCoord ).xyz * 2 - 1 ), 0 );

    vec4 Binormal = vec4( normalize( cross( Normal.xyz, Tangent.xyz ) ), 0 );

    mat4 tbn = uModel * mat4( Tangent, Binormal, Normal, vec4( 0, 0, 0, 1 ) );

    vNormal = uView * tbn * normalSample;
    vTexCoord = TexCoord;
    gl_Position = uProjection * uView * uModel * Position;
}
