#pragma once

#include "Particle.h"
#include <vector>

namespace nsfw {
    namespace particles {

        class ParticleEmitter {
        public:
            ParticleEmitter();
            virtual ~ParticleEmitter();

            virtual void Emit();
            virtual void Update();

        protected:
            std::vector<Particle> m_particles;
            virtual bool ParticleSort( const Particle& left, const Particle& right );
            virtual Particle& AddParticle(); // Returns the first dead particle, or adds a new dead particle to the vector and returns it.

        private:

        };

    }
}
