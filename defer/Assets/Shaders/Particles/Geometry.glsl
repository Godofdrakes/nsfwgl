#version 450 core

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

in vec3 position[];
in float lifeTime[];
in float lifeSpan[];

out vec4 Color;

uniform mat4 cametaTransform;
uniform mat4 projectionView;

uniform float size_START;
uniform float size_END;

uniform vec4 color_START;
uniform vec4 color_END;

void main() {
    Color = mix( color_START, color_END, lifeTime[0] / lifeSpan[0] );

    float halfSize = mix( size_START, size_END, lifeTime[0] / lifeSpan[0] ) * 0.5f;

    vec3 corners[4];
    corners[0] = vec3( halfSize, -halfSize, 0 );
    corners[1] = vec3( halfSize, halfSize, 0 );
    corners[2] = -corners[1];
    corners[3] = -corners[0];

    vec3 zAxis = normalize( cametaTransform[3].xyz - position[0] );
    vec3 xAxis = cross( cametaTransform[1].xyz, zAxis );
    vec3 yAxis = cross( zAxis, xAxis );
    mat3 billboard = mat3( xAxis, yAxis, zAxis );

    for( int n = 0; n < 4; ++n ) {
        gl_Position = projectionView * vec4( billboard * corners[n] + position[0], 1 );
        EmitVertex();
    }

}
