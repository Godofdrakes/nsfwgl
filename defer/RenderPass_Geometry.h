#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"
#include "glbinding/gl/gl.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_Geometry : public RenderPass {

        public:
            RenderPass_Geometry( const char* shaderName, const char* fboName ) : RenderPass( shaderName, fboName ) {}

            virtual void prep() override {
                using namespace gl;
                glEnable( GLenum::GL_DEPTH_TEST );
                unsigned int value = Assets::instance().get( fbo );
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, value );
                glClearColor( 0.f, 0.f, 0.f, 0.f );
                glClear( ClearBufferMask::GL_COLOR_BUFFER_BIT | ClearBufferMask::GL_DEPTH_BUFFER_BIT );
                glUseProgram( Assets::instance().get( shader ) );
            }

            virtual void post() override {
                using namespace gl;
                glBindVertexArray( 0 );
                glUseProgram( 0 );
                glDisable( GLenum::GL_DEPTH_TEST );
                glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
            }

            void draw( const Camera& c, const Geometry& g ) {
                using namespace gl;
                setUniform( "uProjection",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( c.GetProjection() ) );
                setUniform( "uView",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( c.GetViewTransform() ) );
                setUniform( "uModel",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( g.transform ) );

                setUniform( "uSpecularLightPower",
                            UNIFORM::FLO1,
                            &g.specPower );

                setUniform( "uAlbedo",
                            UNIFORM::TEX2,
                            g.diffuse );

                glBindVertexArray( Assets::instance().get( g.mesh ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get( g.tris ),
                                GL_UNSIGNED_INT,
                                0 );

            }
        };
    }
}
