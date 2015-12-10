#version 450 core

layout( location = 0 ) in vec3 Position;
layout( location = 1 ) in vec3 Velocity;
layout( location = 2 ) in float LifeTime;
layout( location = 3 ) in float LifeSpan;

out vec3 position;
out vec3 velocity;
out float lifeTime;
out float lifeSpan;

uniform float time;
uniform float deltaTime;
uniform float lifeSpan_MIN;
uniform float lifeSpan_MAX;
uniform vec3 emitterPosition;

const float INVERSE_MAX_UINT = 1.0f / 4294967295.0f;

float rand( uint seed, float range ) {
    uint i = ( seed ^ 12345391u ) * 2654435769u;
    i ^= ( i << 6u ) ^ ( i >> 26u );
    i *= 2654435769u;
    i += ( i << 5u ) ^ ( i >> 12u );
    return float( range * i ) * INVERSE_MAX_UINT;
}

void main() {
    position = Position + Velocity * deltaTime;
    velocity = Velocity;
    lifeTime = LifeTime + deltaTime;
    lifeSpan = LifeSpan;

    if( lifeTime > lifeSpan ) {
        uint seed = uint( time * 1000.0 ) + uint( gl_VertexID );
        velocity.x = rand( ++seed, 2 ) - 1;
        velocity.y = rand( ++seed, 2 ) - 1;
        velocity.z = rand( ++seed, 2 ) - 1;
        velocity = normalize( velocity );
        position = emitterPosition;
        lifeTime = 0;
        lifeSpan = rand( ++seed, lifeSpan_MAX - lifeSpan_MIN  ) + lifeSpan_MIN;
    }
}
