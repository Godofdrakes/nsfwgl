#include "ParticleEmitter.h"
#include <glm/ext.hpp>
#include <algorithm>
#include "./../nsfwgl/Window.h"

namespace nsfw {
    namespace particles {

        ParticleEmitter::ParticleEmitter() :
            settings(), maxParticles( 25 ),
            m_particleCount( 0 ),
            m_vao( 0 ), m_vbo( 0 ), m_ibo( 0 ),
            m_particles( maxParticles ), dirty( false ) { }

        ParticleEmitter::~ParticleEmitter() { }

        void ParticleEmitter::Emit() {
            Particle& newParticle = AddParticle( settings );
        }

        void ParticleEmitter::Update() {
            float delta = Window::instance().GetDeltaTime();
            bool shouldSort = false;
            for ( auto iter = m_particles.begin(); iter != m_particles.end(); ++iter ) {
                Particle& p = *iter;
                if ( !p.IsAlive() ) {
                    shouldSort = true;
                    continue;
                }
                // Logic
                p.Position += glm::normalize( p.direction ) * p.Get_Speed();
                p.lifeTime += delta;
            }
            if ( shouldSort ) {
                std::sort( m_particles.begin(), m_particles.end(), []( const Particle& a, const Particle& b ) {
                               return a.IsAlive() < b.IsAlive();
                           } );
                m_particleCount = 0;
                for ( auto iter = m_particles.begin(); iter != m_particles.end(); ++iter ) {
                    if ( (*iter).IsAlive() == true ) {
                        ++m_particleCount;
                    }
                }
            }
        }

        Particle& ParticleEmitter::AddParticle( Particle settings ) {
            for ( auto iter = m_particles.begin(); iter != m_particles.end(); ++iter ) {
                if ( (*iter).IsAlive() == true ) {
                    continue;
                }
                return *iter = settings;
            }
            m_particles.push_back( settings );
            return m_particles.back();
        }

    }
}

