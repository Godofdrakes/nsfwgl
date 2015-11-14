#include "ParticleEmitter.h"
#include <algorithm>

namespace nsfw {
    namespace particles {

        ParticleEmitter::ParticleEmitter() : m_particles() { }

        ParticleEmitter::~ParticleEmitter() {}

        void ParticleEmitter::Emit() {}

        void ParticleEmitter::Update() {
            bool shouldSort = false;
            for ( auto iter = m_particles.begin(); iter != m_particles.end(); ++iter ) {
                Particle& p = *iter;
                // Logic
                if ( !p.alive ) {
                    shouldSort = true;
                }
            }
            if ( shouldSort ) {
                std::sort( m_particles.begin(), m_particles.end(), ParticleSort );
            }
        }

        bool ParticleEmitter::ParticleSort( const Particle& left, const Particle& right ) {
            return left.alive;
        }

        Particle& ParticleEmitter::AddParticle() {
            for ( auto iter = m_particles.begin(); iter != m_particles.end(); ++iter ) {
                if ( (*iter).alive == false ) {
                    return *iter;
                }
            }
            m_particles.push_back( Particle() );
            return m_particles.back();
        }

    }
}
