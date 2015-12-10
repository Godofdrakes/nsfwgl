#pragma once

#ifndef ParticleEmitter_GPU_H
#define ParticleEmitter_GPU_H

#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "Transform.h"

namespace nsfw {
    namespace particles {

        struct Particle_GPU {
            glm::vec3 position, velocity;
            float lifeTime, lifeSpan;

            Particle_GPU() :
                position( 0.0f ), velocity( 0.0f ),
                lifeTime( 1.0f ), lifeSpan( 0.0f ) {}
        };

        struct ParticleEmitter_GPU_Settings {
            float lifeSpan_MIN, lifeSpan_MAX;
            float velocity_MIN, velocity_MAX;
            float size_START, size_END;
            glm::vec4 color_START, color_END;

            ParticleEmitter_GPU_Settings() :
                ParticleEmitter_GPU_Settings( 1.0f, 2.0f,
                                              1.0f, 2.0f,
                                              2.0f, 1.0f,
                                              glm::vec4( 1.0f ),
                                              glm::vec4( 0.5f, 0.5f, 1.0f, 1.0f ) ) {};

            ParticleEmitter_GPU_Settings( float lifeSpanMin, float lifeSpanMax, float velocityMin, float velocityMax, float sizeStart, float sizeEnd, glm::vec4 colorStart, glm::vec4 colorEnd )
                : lifeSpan_MIN( lifeSpanMin ), lifeSpan_MAX( lifeSpanMax ),
                  velocity_MIN( velocityMin ), velocity_MAX( velocityMax ),
                  size_START( sizeStart ), size_END( sizeEnd ),
                  color_START( colorStart ), color_END( colorEnd ) {}
        };

        class ParticleEmitter_GPU : public Transform {
        public:
            ParticleEmitter_GPU( const unsigned int maxParticles = 100 ) : MAX_PARTICLES( maxParticles ), m_settings(), m_vao( { 0,0 } ), m_vbo( { 0,0 } ), m_activeBuffer( 0 ) { }

            ~ParticleEmitter_GPU() {
                DestroyParticleBuffers();
            }

            void Init( ParticleEmitter_GPU_Settings settings = ParticleEmitter_GPU_Settings() ) {
                CreateParticleBuffers();
                CreateUpdateShader();
                CreateDrawShader();
                m_settings = settings;
                m_activeBuffer = 0;
            }

            void Draw( float deltaTime, float time, const glm::mat4 cametaTransform, const glm::mat4 projectionView ) {
                ShaderUpdate( deltaTime, time );
                ShaderDraw( cametaTransform, projectionView );
            }

            const unsigned int MAX_PARTICLES;

        private:
            static unsigned int m_updateShader, m_drawShader;
            ParticleEmitter_GPU_Settings m_settings;
            std::array<unsigned int, 2> m_vao, m_vbo;
            unsigned int m_activeBuffer;

            void CreateParticleBuffers();
            void DestroyParticleBuffers();

            static void CreateUpdateShader();
            static void CreateDrawShader();

            void ShaderUpdate( float deltaTime, float time );
            void ShaderDraw( const glm::mat4 cametaTransform, const glm::mat4 projectionView );

        };
    }
}

#endif // ParticleEmitter_GPU_H


