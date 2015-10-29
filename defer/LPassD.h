#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass {
    nsfw::Asset<TEXTURE> normal;

public:
    LPassD( const char* shaderName, const char* fboName ) :
        RenderPass( shaderName, fboName ),
        normal( "GPassNormal" ) {}

    virtual void prep() override {
        using namespace nsfw;
        using namespace gl;
        unsigned int value = Assets::instance().get( fbo );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, value );
        glClearColor( 0.f, 0.f, 0.f, 1.f );
        glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT );
        glEnable( GLenum::GL_BLEND );
        glBlendFunc( GL_ONE, GL_ONE );
        glUseProgram( Assets::instance().get( shader ) );
    }

    virtual void post() override {
        using namespace gl;
        glBindVertexArray( 0 );
        glUseProgram( 0 );
        glDisable( GLenum::GL_BLEND );
        glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
    }

    void draw( const Camera& c, const LightD& l ) {
        using namespace nsfw;
        using namespace gl;

        setUniform( "uLightDirection",
                    UNIFORM::TYPE::FLO3,
                    value_ptr( l.direction ) );
        setUniform( "uLightDiffuse",
                    UNIFORM::TYPE::FLO3,
                    value_ptr( l.color ) );

        setUniform( "uNormalTexture",
                    UNIFORM::TYPE::TEX2,
                    normal,
                    1 );

        glBindVertexArray( Assets::instance().get<VAO>( "Quad" ) );
        glDrawElements( GLenum::GL_TRIANGLES,
                        Assets::instance().get<SIZE>( "Quad" ),
                        GL_UNSIGNED_INT,
                        0 );
    }
};
