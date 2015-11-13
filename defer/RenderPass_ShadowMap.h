#pragma once

#include "../nsfwgl/nsfw.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_ShadowMap : public RenderPass {
            Asset<ASSET::TEXTURE> depth;

        public:
            RenderPass_ShadowMap( const char* shaderName, const char* fboName )
                : RenderPass( shaderName, fboName ), depth( "ShadowPass" ) {}

            virtual void prep() override {
                using namespace gl;
                glEnable( GLenum::GL_DEPTH_TEST );
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, Assets::instance().get( fbo ) );
                glClear( ClearBufferMask::GL_DEPTH_BUFFER_BIT );
                glUseProgram( Assets::instance().get( shader ) );
                glViewport( 0, 0, 1024, 1024 );
            }

            virtual void post() override {
                using namespace gl;
                glViewport( 0, 0, Window::instance().getWidth(), Window::instance().getHeight() );
                glBindVertexArray( 0 );
                glUseProgram( 0 );
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
                glDisable( GLenum::GL_DEPTH_TEST );
            }

            void draw( const lights::Light_Directional& l, const Geometry& g ) {
                using namespace gl;

                setUniform( "uLightMatrix",
                            UNIFORM::MAT4,
                            glm::value_ptr( l.GetLightMatrix() ) );

                setUniform( "uModel",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( g.GetWorldTransform() ) );

                glBindVertexArray( Assets::instance().get( g.mesh ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get( g.tris ),
                                GL_UNSIGNED_INT,
                                0 );
            }
        };
    }
}
