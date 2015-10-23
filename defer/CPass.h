#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"

class CPass : public nsfw::RenderPass {
    nsfw::Asset<nsfw::ASSET::TEXTURE> albedo, position, normal, depth, light;

public:

    CPass( const char* shaderName, const char* fboName )
        : RenderPass( shaderName, fboName ), albedo( "GPassAlbedo" ), position( "GPassPosition" ), // NAMES ARE FROM ASSET LIBRARY!
          normal( "GPassNormal" ), depth( "GPassDepth" ), light( "LPassColor" ) {}


    void prep() {
        using namespace nsfw;
        using namespace gl;
        unsigned int value = Assets::instance().get( fbo );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, value );
        glClearColor( 1.f, 1.f, 1.f, 1.f );
        glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT );
        glUseProgram( Assets::instance().get( shader ) );
    }

    void post() {
        using namespace gl;
        glBindVertexArray( 0 );
        glUseProgram( 0 );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
    }


    void draw() {
        using namespace nsfw;
        using namespace gl;
        setUniform( "uAlbedoTexture",
                    UNIFORM::TEX2,
                    albedo,
                    0 );
        /*setUniform( "uPositionTexture",
                    nsfw::UNIFORM::TEX2,
                    position,
                    1 );
        setUniform( "uNormalTexture",
                    nsfw::UNIFORM::TEX2,
                    normal,
                    2 );
        setUniform( "Depth",
                    nsfw::UNIFORM::TEX2,
                    depth,
                    3 );*/
        setUniform( "uLightTexture",
                    UNIFORM::TEX2,
                    light,
                    1 );

        /*setUniform( "TexelScalar",
                    nsfw::UNIFORM::MAT4,
                    glm::value_ptr( nsfw::Window::instance().getTexelAdjustmentMatrix() ) );*/

        glBindVertexArray( Assets::instance().get<VAO>( "Quad" ) );
        glDrawElements( GLenum::GL_TRIANGLES,
                        Assets::instance().get<SIZE>( "Quad" ),
                        GL_UNSIGNED_INT,
                        0 );
    }
};
