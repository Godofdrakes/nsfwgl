#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"

class GPass : public nsfw::RenderPass {


public:
    void prep() {
#pragma message ( __WARN__ "glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...")
    }

    void post() {
#pragma message ( __WARN__ "Unset any gl settings")
    }

    GPass( const char* shaderName, const char* fboName ) : RenderPass( shaderName, fboName ) {}

    void draw( const Camera& c, const Geometry& g ) {
        setUniform( "Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr( c.getProjection() ) );
        setUniform( "View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr( c.getView() ) );
        setUniform( "Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr( g.transform ) );

        setUniform( "Diffuse", nsfw::UNIFORM::TEX2, g.diffuse, 0 );
        setUniform( "Normal", nsfw::UNIFORM::TEX2, g.normal, 1 );
        setUniform( "Specular", nsfw::UNIFORM::TEX2, g.specular, 2 );

        setUniform( "SpecularPower", nsfw::UNIFORM::FLO1, ( void* )&g.specPower );


        nsfw::Assets::instance().get( g.mesh );
        nsfw::Assets::instance().get( g.tris );

        *g.mesh;
        *g.tris;

#pragma message ( __WARN__ "bindVAO and Draw Elements!")
    }
};
