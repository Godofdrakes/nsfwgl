#pragma once

#include <glm\ext.hpp>

namespace nsfw {

    struct Vertex {
        glm::vec4 position;
        glm::vec4 normal;
        glm::vec4 tangent;
        glm::vec2 texCoord;
    };

    const glm::vec4 up( 0, 1, 0, 0 );
    const glm::vec4 down = -up;
    const glm::vec4 right( 1, 0, 0, 0 );
    const glm::vec4 left = -right;
    const glm::vec4 forward( 0, 0, 1, 0 );
    const glm::vec4 backward = -forward;

    const Vertex CubeVerts[] =
    // Bottom
    { { glm::vec4( -1, -1, 1, 1 ), down, right, glm::vec2( 0, 1 ) },
        { glm::vec4( 1, -1, 1, 1 ), down, right, glm::vec2( 1, 1 ) },
        { glm::vec4( 1, -1, -1, 1 ), down, right, glm::vec2( 1, 0 ) },
        { glm::vec4( -1, -1, -1, 1 ), down, right, glm::vec2( 0, 0 ) },
        // Top
        { glm::vec4( -1, 1, 1, 1 ), up, right, glm::vec2( 0, 1 ) },
        { glm::vec4( 1, 1, 1, 1 ), up, right, glm::vec2( 1, 1 ) },
        { glm::vec4( 1, 1, -1, 1 ), up, right, glm::vec2( 1, 0 ) },
        { glm::vec4( -1, 1, -1, 1 ), up, right, glm::vec2( 0, 0 ) },
        // back
        { glm::vec4( -1, 1, -1, 1 ), backward, left, glm::vec2( 0, 1 ) },
        { glm::vec4( 1, 1, -1, 1 ), backward, left, glm::vec2( 1, 1 ) },
        { glm::vec4( 1, -1, -1, 1 ), backward, left, glm::vec2( 1, 0 ) },
        { glm::vec4( -1, -1, -1, 1 ), backward, left, glm::vec2( 0, 0 ) },
        // front
        { glm::vec4( -1, 1, 1, 1 ), forward, right, glm::vec2( 0, 1 ) },
        { glm::vec4( 1, 1, 1, 1 ), forward, right, glm::vec2( 1, 1 ) },
        { glm::vec4( 1, -1, 1, 1 ), forward, right, glm::vec2( 1, 0 ) },
        { glm::vec4( -1, -1, 1, 1 ), forward, right, glm::vec2( 0, 0 ) },
        // left
        { glm::vec4( -1, 1, -1, 1 ), left, forward, glm::vec2( 1, 0 ) },
        { glm::vec4( -1, 1, 1, 1 ), left, forward, glm::vec2( 1, 1 ) },
        { glm::vec4( -1, -1, 1, 1 ), left, forward, glm::vec2( 0, 1 ) },
        { glm::vec4( -1, -1, -1, 1 ), left, forward, glm::vec2( 0, 0 ) },
        // right
        { glm::vec4( 1, 1, -1, 1 ), right, backward, glm::vec2( 1, 0 ) },
        { glm::vec4( 1, 1, 1, 1 ), right, backward, glm::vec2( 1, 1 ) },
        { glm::vec4( 1, -1, 1, 1 ), right, backward, glm::vec2( 0, 1 ) },
        { glm::vec4( 1, -1, -1, 1 ), right, backward, glm::vec2( 0, 0 ) },
    };

    const unsigned CubeTris[] = {
        3,2,0,0,2,1,
        7,4,6,6,4,5,
        11,8,10,10,8,9,
        15, 14, 12, 12, 14, 13,
        19,18,16,16,18,17,
        23,20,22,22,20,21 };


    const Vertex QuadVerts[] =
    {
        { glm::vec4( -1, 1, 0, 1 ), backward, left, glm::vec2( 0, 1 ) },
        { glm::vec4( 1, 1, 0, 1 ), backward, left, glm::vec2( 1, 1 ) },
        { glm::vec4( 1, -1, 0, 1 ), backward, left, glm::vec2( 1, 0 ) },
        { glm::vec4( -1, -1, 0, 1 ), backward, left, glm::vec2( 0, 0 ) },
    };

    const unsigned QuadTris[] = { 3,1,0, 3,2,1 };

}
