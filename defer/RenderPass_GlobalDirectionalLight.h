#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_GlobalDirectionalLight : public RenderPass {
            Asset<ASSET::TEXTURE> position, normal;

        public:
            RenderPass_GlobalDirectionalLight( const char* shaderName, const char* fboName ) :
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

            void draw( const camera::Camera& c, const lights::Light_Directional& l ) {
                using namespace gl;

                setUniform( "uDirectionalLight.direction",
                            UNIFORM::TYPE::FLO3,
                            glm::value_ptr( l.Position ) );
                setUniform( "uDirectionalLight.color",
                            UNIFORM::TYPE::FLO3,
                            glm::value_ptr( l.color ) );

                setUniform( "uAmbientLightColor",
                            UNIFORM::TYPE::FLO3,
                            glm::value_ptr( glm::vec3( 0.0f, 0.0f, 0.0f ) ) );

                setUniform( "uCameraView",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( c.GetViewTransform() ) );
                setUniform( "uCameraPosition",
                            UNIFORM::TYPE::FLO3,
                            glm::value_ptr( c.Position ) );

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
