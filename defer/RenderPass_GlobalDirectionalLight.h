#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

namespace nsfw {
    namespace rendering {
        class RenderPass_GlobalDirectionalLight : public RenderPass {
            Asset<ASSET::TEXTURE> m_position, m_normal, m_shadowMap;

        public:
            gl::GLboolean enableShadows = gl::GLboolean::GL_TRUE;
            gl::GLfloat shadowBias = 0.01f;

            RenderPass_GlobalDirectionalLight( const char* shaderName, const char* fboName ) :
                RenderPass( shaderName, fboName ),
                m_position( "GPassPosition" ),
                m_normal( "GPassNormal" ),
                m_shadowMap( "ShadowDepth" ) {}

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
                            UNIFORM::FLO3,
                            glm::value_ptr( l.Position ) );
                setUniform( "uDirectionalLight.color",
                            UNIFORM::FLO3,
                            glm::value_ptr( l.color ) );

                setUniform( "uAmbientLightColor",
                            UNIFORM::FLO3,
                            glm::value_ptr( glm::vec3( 0.0f, 0.0f, 0.0f ) ) );

                setUniform( "uCameraView",
                            UNIFORM::MAT4,
                            glm::value_ptr( c.GetViewTransform() ) );
                setUniform( "uCameraPosition",
                            UNIFORM::FLO3,
                            glm::value_ptr( c.Position ) );

                setUniform( "uNormalTexture",
                            UNIFORM::TEX2,
                            m_normal,
                            0 );
                setUniform( "uPositionTexture",
                            UNIFORM::TEX2,
                            m_position,
                            1 );

                setUniform( "uLightMatrix",
                            UNIFORM::MAT4,
                            glm::value_ptr( l.textureSpaceOffset * l.GetLightMatrix() ) );

                if ( enableShadows == GLboolean::GL_TRUE ) {
                    setUniform( "uEnableShadows",
                                UNIFORM::INT1,
                                &enableShadows );
                    setUniform( "uShadowMap",
                                UNIFORM::TEX2,
                                m_shadowMap,
                                2 );
                    setUniform( "uShadowBias",
                                UNIFORM::FLO1,
                                &shadowBias );
                }

                glBindVertexArray( Assets::instance().get<ASSET::VAO>( "Quad" ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get<ASSET::SIZE>( "Quad" ),
                                GL_UNSIGNED_INT,
                                0 );
            }
        };
    }
}
