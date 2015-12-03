#pragma once

#include "../nsfwgl/nsfw.h"
#include "Camera.h"
#include "Geometry.h"
#include "RenderPass_Geometry.h"
#include "RenderPass_Composite.h"
#include "RenderPass_GlobalDirectionalLight.h"
#include "RenderPass_PointLight.h"
#include "RenderPass_ShadowMap.h"

namespace nsfw {
    namespace application {

        class DeferredApplication : public Application {
            camera::Camera* m_camera;
            Geometry* m_soulspear;
            lights::Light_Directional* m_directional;

            rendering::RenderPass_Geometry* m_geometryPass;
            rendering::RenderPass_GlobalDirectionalLight* m_directionalLightPass;
            rendering::RenderPass_Composite* m_compositePass;
            rendering::RenderPass_ShadowMap* m_shadowShader;

            particles::ParticleEmitter* m_particleEmitter;

        public:
            virtual void onInit() override;
            virtual void onStep() override;
            virtual void onTerm() override;
            virtual void onPlay() override;
        };

    }
}
