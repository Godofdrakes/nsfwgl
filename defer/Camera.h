#pragma once

#include "Transform.h"

namespace nsfw {

    namespace camera {

        class Camera : public Transform {
        private:
        protected:
            bool m_isOrtho;
            float m_near, m_far, m_width, m_height, m_fov;

        public:
            bool m_lookAt; // GetWorldTransform() == return glm::lookAt( m_position, m_scale, glm::vec3( 0, 1, 0 ) )

            Camera( glm::vec3 position, float fov = 80.f, float width = 1600.f, float height = 900.f, float near = 0.1f, float far = 1000.0f ) :
                Transform(), m_isOrtho( false ), m_near( near ), m_far( far ), m_width( width ), m_height( height ), m_fov( fov ), m_lookAt( false ) {
                m_position = position;
            }

            virtual ~Camera() {}

            virtual void Update() {}

            virtual glm::mat4 GetWorldTransform() const override {
                if ( m_lookAt ) {
                    return glm::inverse( glm::lookAt( m_position, m_scale, glm::vec3( 0, 1, 0 ) ) );
                }
                return Transform::GetWorldTransform();
            }

            virtual glm::mat4 GetViewTransform() const {
                return glm::inverse( GetWorldTransform() );
            }

            virtual glm::mat4 GetProjection() const {
                if ( m_isOrtho ) {
                    float halfWidth = m_width * 0.5f;
                    float halfHeight = m_height * 0.5f;
                    return glm::ortho( -halfWidth, halfWidth, -halfHeight, halfWidth, m_near, m_far );
                }
                return glm::perspective( m_fov, m_width / m_height, m_near, m_far );
            }
        };

    }

}
