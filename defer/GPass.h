#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"
#include "glbinding/gl/gl.h"

class GPass : public nsfw::RenderPass {


public:
    GPass( const char* shaderName, const char* fboName ) : RenderPass( shaderName, fboName ) {}

    void prep() {
        using namespace gl;
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER , *fbo );
        glEnable( GLenum::GL_DEPTH_TEST );
        glClearColor( 0.f, 0.f, 0.f, 1.f );
        glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT | ClearBufferMask::GL_DEPTH_BUFFER_BIT );
        glUseProgram( *shader );
    }

    void post() {
        using namespace gl;
        glBindVertexArray( 0 );
        glUseProgram( 0 );
        glDisable( GLenum::GL_DEPTH_TEST );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
    }

    void draw( const Camera& c, const Geometry& g ) {
        using namespace gl;
        setUniform( "uProjection",
                    nsfw::UNIFORM::TYPE::MAT4,
                    glm::value_ptr( c.getProjection() ) );
        setUniform( "uView",
                    nsfw::UNIFORM::TYPE::MAT4,
                    glm::value_ptr( c.getView() ) );
        /*setUniform( "uModel",
                    nsfw::UNIFORM::TYPE::MAT4,
                    glm::value_ptr( g.transform ) );*/

        setUniform( "uDiffuse",
                    nsfw::UNIFORM::TEX2,
                    g.diffuse );
        /*setUniform( "Normal",
                    nsfw::UNIFORM::TEX2,
                    g.normal,
                    1 );
        setUniform( "Specular",
                    nsfw::UNIFORM::TEX2,
                    g.specular,
                    2 );

        setUniform( "SpecularPower",
                    nsfw::UNIFORM::FLO1,
                    ( void* )&g.specPower );*/

        glBindVertexArray( *g.mesh );
        glDrawElements( GLenum::GL_TRIANGLES,
                        *g.tris,
                        GL_UNSIGNED_INT,
                        0 );

    }
};
