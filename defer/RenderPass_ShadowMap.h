#pragma once

#include "../nsfwgl/nsfw.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_ShadowMap : public RenderPass {
            Asset<TEXTURE> depth;

        public:
            RenderPass_ShadowMap( const char* shaderName, const char* fboName )
                : RenderPass( shaderName, fboName ), depth( "ShadowMap" ) {}

            virtual void prep() override {
                using namespace gl;
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, Assets::instance().get( fbo ) );
                glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT | ClearBufferMask::GL_DEPTH_BUFFER_BIT );
                glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
                glUseProgram( Assets::instance().get( shader ) );
            }

            virtual void post() override {
                using namespace gl;
                glBindVertexArray( 0 );
                glUseProgram( 0 );
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
            }

            void draw() {
                using namespace gl;

                glBindVertexArray( Assets::instance().get<VAO>( "Quad" ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get<SIZE>( "Quad" ),
                                GL_UNSIGNED_INT,
                                0 );
            }
        };
    }
}
