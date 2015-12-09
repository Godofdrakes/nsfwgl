#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"
#include "glbinding/gl/gl.h"
#include "ParticleEmitter.h"

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

            void draw( const camera::Camera& c, const Geometry& g ) {
                setUniform( "uProjection",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( c.GetProjection() ) );
                setUniform( "uView",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( c.GetViewTransform() ) );
                setUniform( "uModel",
                            UNIFORM::TYPE::MAT4,
                            glm::value_ptr( g.GetWorldTransform() ) );

                setUniform( "uAlbedo",
                            UNIFORM::TEX2,
                            g.diffuse,
                            0 );
                setUniform( "uNormalMap",
                            UNIFORM::TEX2,
                            g.normal,
                            1 );

                const gl::GLint thing = 1;
                setUniform( "uUseTexture",
                            UNIFORM::INT1,
                            &thing );

                setUniform( "uSpecularLightPower",
                            UNIFORM::FLO1,
                            &g.specPower );

                setUniform( "uColor",
                            UNIFORM::FLO4,
                            glm::value_ptr( glm::vec4( 1.0f ) ) );

                using namespace gl;
                glBindVertexArray( Assets::instance().get( g.mesh ) );
                glDrawElements( GLenum::GL_TRIANGLES,
                                Assets::instance().get( g.tris ),
                                GL_UNSIGNED_INT,
                                0 );
            }

            void draw( const camera::Camera& c, const particles::ParticleEmitter& p ) {
                for ( auto iter = p.Get_Particles().begin(); iter != p.Get_Particles().end(); ++iter ) {
                    const particles::Particle& particle = *iter;
                    if ( particle.IsAlive() == false ) {
                        continue;
                    }

                    setUniform( "uProjection",
                                UNIFORM::TYPE::MAT4,
                                glm::value_ptr( c.GetProjection() ) );
                    setUniform( "uView",
                                UNIFORM::TYPE::MAT4,
                                glm::value_ptr( c.GetViewTransform() ) );
                    setUniform( "uModel",
                                UNIFORM::TYPE::MAT4,
                                glm::value_ptr( particle.GetWorldTransform() ) );

                    setUniform( "uAlbedo",
                                UNIFORM::TEX2,
                                Asset<ASSET::TEXTURE>( "Fallback_Diffuse" ),
                                0 );
                    setUniform( "uNormalMap",
                                UNIFORM::TEX2,
                                Asset<ASSET::TEXTURE>( "Fallback_Normal" ),
                                1 );

                    const gl::GLint thing = 0;
                    setUniform( "uUseTexture",
                                UNIFORM::INT1,
                                &thing );

                    float specular = 0.0f;
                    setUniform( "uSpecularLightPower",
                                UNIFORM::FLO1,
                                &specular );

                    setUniform( "uColor",
                                UNIFORM::FLO4,
                                glm::value_ptr( particle.Get_Color() ) );

                    using namespace gl;
                    glBindVertexArray( Assets::instance().get<ASSET::VAO>( "Quad" ) );
                    glDrawElements( GLenum::GL_TRIANGLES,
                                    Assets::instance().get<ASSET::SIZE>( "Quad" ),
                                    GL_UNSIGNED_INT,
                                    0 );
                }
            }
        };
    }
}

