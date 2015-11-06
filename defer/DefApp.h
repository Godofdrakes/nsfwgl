#pragma once

#include "../nsfwgl/nsfw.h"
#include "RenderPass_Geometry.h"
#include "RenderPass_Composite.h"
#include "RenderPass_GlobalDirectionalLight.h"
#include "RenderPass_PointLight.h"
#include "RenderPass_ShadowMap.h"

struct LightD;

namespace nsfw {
    namespace camera {
        class Camera;
    }

    class Geometry;

    namespace application {
        class DeferredApplication : public Application {
            camera::Camera* m_camera;
            Geometry* m_soulspear;
            LightD* m_light;

            rendering::RenderPass_Geometry* m_geometryPass;
            rendering::RenderPass_GlobalDirectionalLight* m_pass_GlobalDirectionalLight;
            rendering::RenderPass_Composite* m_compositePass;

        public:
            virtual void onInit() override;
            virtual void onStep() override;
            virtual void onTerm() override;
            virtual void onPlay() override;
        };
    }
}
