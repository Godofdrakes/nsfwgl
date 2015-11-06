#pragma once
#include "Camera.h"
#include "../nsfwgl/Window.h"

namespace nsfw {
    namespace camera {
        class FlyCamera : public Camera {

        public:
            FlyCamera( const glm::vec3& position, float fov = 80.f, float width = 1600.f, float height = 900.f, float near = 0.1f, float far = 1000.0f )
                : Camera( position, fov, width, height, near, far ) {}

            virtual ~FlyCamera() override {};

            virtual void Update() override;
        };
    }
}
