#pragma once

#include "Particle.h"
#include <vector>

namespace nsfw {
    namespace particles {

        class ParticleEmitter {
        public:
            ParticleEmitter();
            virtual ~ParticleEmitter();

            Particle settings;
            unsigned int maxParticles;


            virtual void Emit();
            virtual void Update();

            const std::vector<particles::Particle>& Get_Particles() const {
                return m_particles;
            }

            unsigned int Get_ParticleCount() const {
                return m_particleCount;
            }

        protected:
            unsigned int m_particleCount;
            unsigned int m_vao, m_vbo, m_ibo;
            std::vector<particles::Particle> m_particles;
            Particle& AddParticle( Particle settings = Particle() );

        private:

        };

    }
}
