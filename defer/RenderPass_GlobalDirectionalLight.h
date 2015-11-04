#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class RenderPass_GlobalDirectionalLight : public nsfw::RenderPass {
    nsfw::Asset<TEXTURE> position, normal;

public:
    RenderPass_GlobalDirectionalLight( const char* shaderName, const char* fboName ) :
        RenderPass( shaderName, fboName ),
        position( "GPassPosition" ),
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


        setUniform( "uDirectionalLight.direction",
                    UNIFORM::TYPE::FLO3,
                    value_ptr( l.direction ) );
        setUniform( "uDirectionalLight.color",
                    UNIFORM::TYPE::FLO3,
                    value_ptr( l.color ) );

        /*setUniform( "uAmbientLightColor",
                    UNIFORM::TYPE::FLO3,
                    value_ptr( vec3( 0.1f, 0.1f, 0.1f ) ) );*/

        //float specular = 0; // TODO: Properly implement
        //setUniform( "uSpecularLightPower",
        //            UNIFORM::TYPE::FLO1,
        //            &specular );


        setUniform( "uCameraView",
                    UNIFORM::TYPE::MAT4,
                    value_ptr( c.getView() ) );
        /*setUniform( "uCameraPosition",
                    UNIFORM::TYPE::FLO3,
                    value_ptr( c.worldPosition ) );*/

        setUniform( "uNormalTexture",
                    UNIFORM::TYPE::TEX2,
                    normal,
                    0 );
        setUniform( "uNormalTexture",
                    UNIFORM::TYPE::TEX2,
                    position,
                    1 );

        // TODO: Lighting looks wrong

        glBindVertexArray( Assets::instance().get<VAO>( "Quad" ) );
        glDrawElements( GLenum::GL_TRIANGLES,
                        Assets::instance().get<SIZE>( "Quad" ),
                        GL_UNSIGNED_INT,
                        0 );
    }
};
