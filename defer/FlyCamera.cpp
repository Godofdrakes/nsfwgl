#include "FlyCamera.h"
#include "glfw/glfw3.h"

namespace nsfw {
    namespace camera {

        void FlyCamera::Update() {

            glm::vec3 movement( 0.0f, 0.0f, 0.0f );
            float moveSpeed = 0.25f, rotationSpeed = moveSpeed * 9;

            if ( Window::instance().getKey( 87 ) ) { // W
                movement.z = -moveSpeed;
            }
            else if ( Window::instance().getKey( 83 ) ) { // S
                movement.z = moveSpeed;
            }
            if ( Window::instance().getKey( 65 ) ) { // A
                movement.x = -moveSpeed;
            }
            else if ( Window::instance().getKey( 68 ) ) { // D
                movement.x = moveSpeed;
            }
            if ( Window::instance().getKey( 81 ) ) { // Q
                movement.y = -moveSpeed;
            }
            else if ( Window::instance().getKey( 69 ) ) { // E
                movement.y = moveSpeed;
            }

            m_position = m_position + ( glm::vec3 )(GetRoationMatrix() * glm::vec4( movement, 0 ));

            glm::vec3 rotation( 0.0f, 0.0f, 0.0f );

            if ( Window::instance().getKey( 265 ) ) { // UP
                rotation.x = rotationSpeed;
            }
            else if ( Window::instance().getKey( 264 ) ) { // DOWN
                rotation.x = -rotationSpeed;
            }
            if ( Window::instance().getKey( 263 ) ) { // LEFT
                rotation.y = rotationSpeed;
            }
            else if ( Window::instance().getKey( 262 ) ) { // RIGHT
                rotation.y = -rotationSpeed;
            }

            m_rotation = m_rotation + rotation;
        }
    }
}
