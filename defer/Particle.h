#pragma once

#include "Transform.h"

namespace nsfw {
    namespace particles {

        struct Particle : public Transform {
            glm::vec3 startSpeed, endSpeed;
            glm::vec4 startColor, endColor;
            glm::vec3 startScale, endScale;
            float lifeTime, lifeSpan;

            bool IsAlive() const {
                return lifeTime < lifeSpan;
            }

            Particle( glm::vec3 position = glm::vec3( 0 ),
                      glm::vec3 startSpeed = glm::vec3( 1 ),
                      glm::vec3 endSpeed = glm::vec3( 0 ),
                      glm::vec4 startColor = glm::vec4( 1 ),
                      glm::vec4 endColor = glm::vec4( 0 ),
                      glm::vec3 startScale = glm::vec3( 1 ),
                      glm::vec3 endScale = glm::vec3( 1 ),
                      float lifeTime = 0.0f,
                      float lifeSpan = 0.0f )
                : Transform(),
                  startSpeed( startSpeed ),
                  endSpeed( endSpeed ),
                  startColor( startColor ),
                  endColor( endColor ),
                  startScale( startScale ),
                  endScale( endScale ),
                  lifeTime( lifeTime ),
                  lifeSpan( lifeSpan ) {}

            glm::vec4 Get_Color() const {
                return glm::lerp( startColor, endColor, lifeTime / lifeSpan );
            }

            glm::vec3 Get_Speed() const {
                return glm::lerp( startSpeed, endSpeed, lifeTime / lifeSpan );
            }

            virtual glm::vec3 Get_Scale() const override {
                return glm::lerp( startScale, endScale, lifeTime / lifeSpan );
            }


        };

    }
}
