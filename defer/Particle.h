#pragma once

#include "Transform.h"

namespace nsfw {
    namespace particles {

        struct Particle : public Transform {
            glm::vec3 direction;
            float startSpeed, endSpeed;
            glm::vec4 startColor, endColor;
            glm::vec3 startScale, endScale;
            float lifeTime, lifeSpan;

            bool IsAlive() const {
                return lifeTime < lifeSpan;
            }

            Particle(
                glm::vec3 position = glm::vec3( 0.0f ),
                glm::vec3 direction = glm::vec3( 0.0f, 1.0f, 0.0f ),
                float startSpeed = 0.0f,
                float endSpeed = 0.0f,
                glm::vec4 startColor = glm::vec4( 1.0f ),
                glm::vec4 endColor = glm::vec4( 0.0f ),
                glm::vec3 startScale = glm::vec3( 1.0f ),
                glm::vec3 endScale = glm::vec3( 1.0f ),
                float lifeSpan = 0.0f )
                : Transform( position ),
                  direction( direction ),
                  startSpeed( startSpeed ),
                  endSpeed( endSpeed ),
                  startColor( startColor ),
                  endColor( endColor ),
                  startScale( startScale ),
                  endScale( endScale ),
                  lifeTime( 0.0f ),
                  lifeSpan( lifeSpan ) {}

            glm::vec4 Get_Color() const {
                return glm::vec4( glm::lerp( ( glm::vec3 )startColor, ( glm::vec3 )endColor, Get_LifeNormal() ), 1.0f );
            }

            float Get_Speed() const {
                return startSpeed + Get_LifeNormal() * (endSpeed - startSpeed);
            }

            virtual glm::vec3 Get_Scale() const override sealed {
                return glm::lerp( startScale, endScale, Get_LifeNormal() );
            }

            float Get_LifeNormal() const {
                return lifeTime / lifeSpan;
            }

        };

    }
}

