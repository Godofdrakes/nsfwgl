#pragma once
#include "Camera.h"

namespace nsfw {
    namespace lights {

        // Will point from position towards 0,0,0
        class Light_Directional : public camera::Camera {
        public:
            glm::vec3 color;

            virtual void Set_Position( const glm::vec3 position ) override {
                m_position = glm::normalize( position );
            }

            Light_Directional( const glm::vec3& position, glm::vec3 color = glm::vec3( 1.0f, 1.0f, 1.0f ) )
                : Camera( position, 90.0f, 20, 20, -10, 10 ), color( color ) {
                m_isOrtho = true;
                m_lookAt = true;
            }

            virtual glm::mat4 GetWorldTransform() const override {
                return glm::inverse( glm::lookAt( m_position,
                                                  glm::vec3( 0.0f, 0.0f, 0.0f ),
                                                  glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
            }

            virtual glm::mat4 GetProjection() const override {
                return glm::ortho( -10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f );
            }
        };
    }
}
