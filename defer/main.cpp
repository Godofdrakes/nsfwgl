#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "FlyCamera.h"

#include "RenderPass_Geometry.h"
#include "RenderPass_GlobalDirectionalLight.h"
#include "RenderPass_PointLight.h"
#include "RenderPass_Composite.h"
#include "RenderPass_ShadowMap.h"


int main() {
    using namespace nsfw::application;
    DeferredApplication d;
    d.init();
    d.play();
    d.term();
}

namespace nsfw {
    namespace application {

        void DeferredApplication::onInit() {
            using namespace gl;
            auto& w = Window::instance();
            auto& a = Assets::instance();

            // Setup FBOs
            const char* gpassTextureNames[] = { "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
            const GLenum gpassDepths[] = { GL_RGB8, GL_RGBA32F, GL_RGB32F, GL_DEPTH_COMPONENT };
            a.makeFBO( "GeometryPass", w.getWidth(), w.getHeight(), 4, gpassTextureNames, gpassDepths );

            const char* lpassTextureNames[] = { "LPassColor" };
            const GLenum lpassDepths[] = { GL_RGB8 };
            a.makeFBO( "LightPass", w.getWidth(), w.getHeight(), 1, lpassTextureNames, lpassDepths );

            const char* shadowTextureNames[] = { "ShadowDepth" };
            const GLenum shadowDepths[] = { GL_DEPTH_COMPONENT };
            a.makeFBO( "ShadowPass", w.getWidth() * 2, w.getWidth() * 2, 1, shadowTextureNames, shadowDepths );

            // Load Shaders
            a.loadShader( "GeometryPassPhong",
                          "./Assets/Shaders/Geometry/Vertex.glsl",
                          "./Assets/Shaders/Geometry/Fragment.glsl" );
            a.loadShader( "LightPassDirectional",
                          "./Assets/Shaders/Lighting/Global_Directional/Vertex.glsl",
                          "./Assets/Shaders/Lighting/Global_Directional/Fragment.glsl" );
            a.loadShader( "CompPass",
                          "./Assets/Shaders/Composite/Vertex.glsl",
                          "./Assets/Shaders/Composite/Fragment.glsl" );

            // Load any other textures and geometry we want to use
            a.loadFBX( "Soulspear", "./Assets/FBX/SoulSpear/soulspear.fbx" );
        }

        void DeferredApplication::onPlay() {
            m_camera = new camera::FlyCamera( glm::vec3( 0, 0, 10 ), 80.f, Window::instance().getWidth(), Window::instance().getHeight() );
            m_soulspear = new Geometry[4];

            m_directional = new lights::Light_Directional( glm::vec3( 0.0f, 1.0f, 1.0f ) );

#pragma message ( "Make sure the following names match the FBX file's output!" )
            m_soulspear[0].mesh = "Soulspear";
            m_soulspear[0].tris = "Soulspear";
            m_soulspear[0].diffuse = "soulspear_diffuse.tga"; // loadFBX will need to name every handle it creates,
            m_soulspear[0].normal = "soulspear_normal.tga"; // These handle names may not be what your loadFBX sets them as!
            m_soulspear[0].specular = "soulspear_specular.tga"; // (Assets will report what the key names are though)
            m_soulspear[0].specPower = 64.0f;
            m_soulspear[0].Position = glm::vec3( 0, 0, 0 );

            m_soulspear[1].mesh = "Soulspear";
            m_soulspear[1].tris = "Soulspear";
            m_soulspear[1].diffuse = "soulspear_diffuse.tga";
            m_soulspear[1].normal = "soulspear_normal.tga";
            m_soulspear[1].specular = "soulspear_specular.tga";
            m_soulspear[1].specPower = 256.0f;
            m_soulspear[1].Position = glm::vec3( 5, 1, 0 );

            m_soulspear[2].mesh = "Soulspear";
            m_soulspear[2].tris = "Soulspear";
            m_soulspear[2].diffuse = "soulspear_diffuse.tga";
            m_soulspear[2].normal = "soulspear_normal.tga";
            m_soulspear[2].specular = "soulspear_specular.tga";
            m_soulspear[2].specPower = 0.0f;
            m_soulspear[2].Position = glm::vec3( -5, -1, 0 );

            m_soulspear[3].mesh = "Quad";
            m_soulspear[3].tris = "Quad";
            m_soulspear[3].diffuse = "Fallback_White";
            m_soulspear[3].normal = "Fallback_Black";
            m_soulspear[3].specular = "Fallback_Black";
            m_soulspear[3].specPower = 0.0f;
            m_soulspear[3].Position = glm::vec3( 0, -3, 0 );
            m_soulspear[3].Rotation = glm::vec3( 90.0f, 0.0f, 0.0f );
            m_soulspear[3].Scale = glm::vec3( 20, 20, 1 );

            m_geometryPass = new rendering::RenderPass_Geometry( "GeometryPassPhong", "GeometryPass" );
            m_directionalLightPass = new rendering::RenderPass_GlobalDirectionalLight( "LightPassDirectional", "LightPass" );
            m_compositePass = new rendering::RenderPass_Composite( "CompPass", "Screen" ); // Screen is defined in nsfw::Assets::init()
        }

        void DeferredApplication::onStep() {

            m_directional->Update();
            m_directional->Position = glm::normalize( glm::vec3( sin( Window::instance().getTime() ), m_directional->Position.y, m_directional->Position.z ) );

            m_camera->Update();
            m_soulspear->Update();

            m_geometryPass->prep();
            m_geometryPass->draw( *m_camera, m_soulspear[0] );
            m_geometryPass->draw( *m_camera, m_soulspear[1] );
            m_geometryPass->draw( *m_camera, m_soulspear[2] );
            m_geometryPass->draw( *m_camera, m_soulspear[3] );
            m_geometryPass->post();

            m_directionalLightPass->prep();
            m_directionalLightPass->draw( *m_camera, *m_directional );
            m_directionalLightPass->post();

            m_compositePass->prep();
            m_compositePass->draw();
            m_compositePass->post();
        }

        void DeferredApplication::onTerm() {
            delete m_camera;
            delete m_directional;
            delete[] m_soulspear;

            delete m_compositePass;
            delete m_geometryPass;
            delete m_directionalLightPass;
        }
    }
}
