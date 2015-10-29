#pragma once

#include "../nsfwgl/nsfw.h"

class CPass : public nsfw::RenderPass {
    nsfw::Asset<TEXTURE> albedo, depth, light;

public:
    CPass( const char* shaderName, const char* fboName )
        : RenderPass( shaderName, fboName ), albedo( "GPassAlbedo" ), // NAMES ARE FROM ASSET LIBRARY!
          depth( "GPassDepth" ), light( "LPassColor" ) {}


    virtual void prep() override {
        using namespace nsfw;
        using namespace gl;
        unsigned int value = Assets::instance().get( fbo );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, value );
        glClearColor( 1.f, 1.f, 1.f, 1.f );
        glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT );
        glUseProgram( Assets::instance().get( shader ) );
    }

    virtual void post() override {
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

        setUniform( "uLightTexture",
                    UNIFORM::TEX2,
                    light,
                    1 );

        glBindVertexArray( Assets::instance().get<VAO>( "Quad" ) );
        glDrawElements( GLenum::GL_TRIANGLES,
                        Assets::instance().get<SIZE>( "Quad" ),
                        GL_UNSIGNED_INT,
                        0 );
    }
};
