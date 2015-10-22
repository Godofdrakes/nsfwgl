#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass {
    nsfw::Asset<nsfw::ASSET::TEXTURE> position, normal;

public:
    LPassD( const char* shaderName, const char* fboName ) :
        RenderPass( shaderName, fboName ),
        position( "GPassPosition" ),
        normal( "GPassNormal" ) {}

    void prep() {
        using namespace gl;
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, *fbo );
        glClearColor( 1.f, 1.f, 1.f, 1.f );
        glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT );
        glEnable( GLenum::GL_BLEND );
        glBlendFunc( GL_ONE, GL_ONE );
        glUseProgram( *shader );
    }

    void post() {
        using namespace gl;
        glBindVertexArray( 0 );
        glUseProgram( 0 );
        glDisable( GLenum::GL_BLEND );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
    }


    void draw( const Camera& c, const LightD& l ) {
        using namespace gl;
        /*setUniform( "Projection",
                    nsfw::UNIFORM::TYPE::MAT4,
                    glm::value_ptr( c.getProjection() ) );
        setUniform( "View",
                    nsfw::UNIFORM::TYPE::MAT4,
                    glm::value_ptr( c.getView() ) );*/

        setUniform( "uLightDirection",
                    nsfw::UNIFORM::TYPE::FLO3,
                    glm::value_ptr( l.direction ) );
        setUniform( "uLightDiffuse",
                    nsfw::UNIFORM::TYPE::FLO3,
                    glm::value_ptr( l.color ) );

        setUniform( "uPositionTexture",
                    nsfw::UNIFORM::TYPE::TEX2,
                    position,
                    0 );
        setUniform( "uNormalTexture",
                    nsfw::UNIFORM::TYPE::TEX2,
                    normal,
                    1 );

        /*setUniform( "TexelScalar",
                    nsfw::UNIFORM::MAT4,
                    glm::value_ptr( nsfw::Window::instance().getTexelAdjustmentMatrix() ) );*/

    //#pragma message( __ERR__ "Bind uPositionTexture and uNormalTexture" )

        glBindVertexArray( nsfw::Assets::instance().get<nsfw::ASSET::VAO>( "Quad" ) );
        glDrawElements( GLenum::GL_TRIANGLES,
                        nsfw::Assets::instance().get<nsfw::ASSET::SIZE>( "Quad" ),
                        GL_UNSIGNED_INT,
                        0 );
    }
};
