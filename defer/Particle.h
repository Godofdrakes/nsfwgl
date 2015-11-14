#pragma once

#include <glm/glm.hpp>

namespace nsfw {
    namespace particles {

        struct Particle {
            friend class ParticleEmitter;
        public:
            bool alive;
            glm::vec3 position;
            float lifeTime;
            float startSpeed, endSpeed;
            glm::vec3 startScale, endScale;
            glm::vec3 startRotation, endRotation;

        protected:

        private:
            Particle() : alive( false ),
                         position( 0.0f, 0.0f, 0.0f ),
                         lifeTime( 0.0f ),
                         startSpeed( 0.0f ),
                         endSpeed( 0.0f ),
                         startScale( 1.0f, 1.0f, 1.0f ),
                         endScale( 1.0f, 1.0f, 1.0f ),
                         startRotation( 0.0f, 0.0f, 0.0f ),
                         endRotation( 0.0f, 0.0f, 0.0f ) {}

            Particle( const Particle& original ) : Particle() {
                alive = original.alive;
                position = original.position;
                lifeTime = original.lifeTime;
                startSpeed = original.startSpeed;
                endSpeed = original.endSpeed;
                startScale = original.startScale;
                endScale = original.endScale;
                startRotation = original.startRotation;
                endRotation = original.endRotation;
            }

        };

    }
}
