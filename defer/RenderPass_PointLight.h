#pragma once

#include "../nsfwgl/nsfw.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_PointLight : public RenderPass {
            Asset<ASSET::TEXTURE> position, normal;

        public:
            RenderPass_PointLight( const char* shaderName, const char* fboName ) :
                RenderPass( shaderName, fboName ),
                position( "GPassPosition" ),
                normal( "GPassNormal" ) {}

            virtual void prep() override {
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

            void draw( const camera::Camera& c, const LightP& l ) {
                using namespace gl;

                setUniform( "uPointLight.position",
                            UNIFORM::TYPE::FLO3,
                            value_ptr( l.position ) );
                setUniform( "uPointLight.color",
                            UNIFORM::TYPE::FLO3,
                            value_ptr( l.color ) );

                setUniform( "uCameraView",
                            UNIFORM::TYPE::MAT4,
                            value_ptr( c.GetViewTransform() ) );
                setUniform( "uCameraPosition",
                            UNIFORM::TYPE::FLO3,
                            value_ptr( c.m_position ) );

                setUniform( "uNormalTexture",
                            UNIFORM::TYPE::TEX2,
                            normal,
                            0 );
                setUniform( "uPositionTexture",
                            UNIFORM::TYPE::TEX2,
                            position,
                            1 );

                glBindVertexArray( Assets::instance().get<ASSET::VAO>( "Quad" ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get<ASSET::SIZE>( "Quad" ),
                                GL_UNSIGNED_INT,
                                0 );
            }
        };
    }
}
