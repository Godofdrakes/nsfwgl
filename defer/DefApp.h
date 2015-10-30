#pragma once

#include "../nsfwgl/nsfw.h"

struct Camera;
struct Geometry;
struct LightD;

class GPass;
class RenderPass_GlobalDirectionalLight;
class CPass;

class DeferredApplication : public nsfw::Application {
    Camera* m_camera;
    Geometry* m_soulspear;
    LightD* m_light;

    CPass* m_compositePass;
    GPass* m_geometryPass;
    RenderPass_GlobalDirectionalLight* m_pass_GlobalDirectionalLight;

public:
    virtual void onInit() override;
    virtual void onStep() override;
    virtual void onTerm() override;
    virtual void onPlay() override;
};
