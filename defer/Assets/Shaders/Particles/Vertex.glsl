#version 450 core

layout( location = 0 ) in vec3 Position;
layout( location = 1 ) in vec3 Velocity;
layout( location = 2 ) in float LifeTime;
layout( location = 3 ) in float LifeSpan;

out vec3 position;
out float lifeTime;
out float lifeSpan;

void main() {
    position = Position;
    lifeTime = LifeTime;
    lifeSpan = LifeSpan;
}
