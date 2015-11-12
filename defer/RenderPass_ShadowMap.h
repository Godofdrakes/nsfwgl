#pragma once

#include "../nsfwgl/nsfw.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_ShadowMap : public RenderPass {
            Asset<ASSET::TEXTURE> depth;

        public:
            RenderPass_ShadowMap( const char* shaderName, const char* fboName )
                : RenderPass( shaderName, fboName ), depth( "ShadowMap" ) {}

            virtual void prep() override {
                using namespace gl;
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, Assets::instance().get( fbo ) );
                glClear( ClearBufferMask::GL_DEPTH_BUFFER_BIT );
                glUseProgram( Assets::instance().get( shader ) );
            }

            virtual void post() override {
                using namespace gl;
                glBindVertexArray( 0 );
                glUseProgram( 0 );
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
            }

            void draw( const lights::Light_Directional& l, const Geometry& g ) {
                using namespace gl;

                setUniform( "uLightMatrix",
                            UNIFORM::MAT4,
                            glm::value_ptr( l.GetProjection() * l.GetViewTransform() ) );

                glBindVertexArray( Assets::instance().get( g.mesh ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get( g.mesh ),
                                GL_UNSIGNED_INT,
                                0 );
            }
        };
    }
}
