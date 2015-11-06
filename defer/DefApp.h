#pragma once

#include "../nsfwgl/nsfw.h"
#include "RenderPass_Geometry.h"
#include "RenderPass_Composite.h"
#include "RenderPass_GlobalDirectionalLight.h"
#include "RenderPass_PointLight.h"
#include "RenderPass_ShadowMap.h"

struct Camera;
struct Geometry;
struct LightD;

class GPass;
class RenderPass_GlobalDirectionalLight;
class CPass;

namespace nsfw {
    namespace application {
        using namespace rendering;

        class DeferredApplication : public Application {
            Camera* m_camera;
            Geometry* m_soulspear;
            LightD* m_light;

            RenderPass_Geometry* m_geometryPass;
            RenderPass_GlobalDirectionalLight* m_pass_GlobalDirectionalLight;
            RenderPass_Composite* m_compositePass;

        public:
            virtual void onInit() override;
            virtual void onStep() override;
            virtual void onTerm() override;
            virtual void onPlay() override;
        };
    }
}
