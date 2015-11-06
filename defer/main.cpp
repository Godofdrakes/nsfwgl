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
            m_camera = new camera::FlyCamera( glm::vec3( 0, 0, 10 ) );
            m_soulspear = new Geometry[4];

            m_light = new LightD;
            m_light->color = vec3( 1.0f, 1.0f, 1.0f ); // Make sure the light is coming from a direction that makes the world visible to the user
            m_light->direction = normalize( vec3( 0.f, 0.f, 1.f ) );

#pragma message ( "Make sure the following names match the FBX file's output!" )
            m_soulspear[0].mesh = "Soulspear";
            m_soulspear[0].tris = "Soulspear";
            m_soulspear[0].diffuse = "soulspear_diffuse.tga"; // loadFBX will need to name every handle it creates,
            m_soulspear[0].normal = "soulspear_normal.tga"; // These handle names may not be what your loadFBX sets them as!
            m_soulspear[0].specular = "soulspear_specular.tga"; // (Assets will report what the key names are though)
            m_soulspear[0].specPower = 64.0f;
            m_soulspear[0].m_position = glm::vec3( 0, 0, 0 );

            m_soulspear[1].mesh = "Soulspear";
            m_soulspear[1].tris = "Soulspear";
            m_soulspear[1].diffuse = "soulspear_diffuse.tga";
            m_soulspear[1].normal = "soulspear_normal.tga";
            m_soulspear[1].specular = "soulspear_specular.tga";
            m_soulspear[1].specPower = 128.0f;
            m_soulspear[1].m_position = glm::vec3( 5, 1, 0 );

            m_soulspear[2].mesh = "Soulspear";
            m_soulspear[2].tris = "Soulspear";
            m_soulspear[2].diffuse = "soulspear_diffuse.tga";
            m_soulspear[2].normal = "soulspear_normal.tga";
            m_soulspear[2].specular = "soulspear_specular.tga";
            m_soulspear[2].specPower = 0.0f;
            m_soulspear[2].m_position = glm::vec3( -5, -1, 0 );

            m_soulspear[3].mesh = "Quad";
            m_soulspear[3].tris = "Quad";
            m_soulspear[3].diffuse = "Fallback_White";
            m_soulspear[3].normal = "Fallback_Black";
            m_soulspear[3].specular = "Fallback_Black";
            m_soulspear[3].specPower = 0.0f;
            m_soulspear[3].m_position = glm::vec3( 0, -3, 0 );
            m_soulspear[3].m_rotation = glm::vec3( 90.0f, 0.0f, 0.0f );
            m_soulspear[3].m_scale = glm::vec3( 10, 10, 1 );

            m_geometryPass = new rendering::RenderPass_Geometry( "GeometryPassPhong", "GeometryPass" );
            m_pass_GlobalDirectionalLight = new rendering::RenderPass_GlobalDirectionalLight( "LightPassDirectional", "LightPass" );
            m_compositePass = new rendering::RenderPass_Composite( "CompPass", "Screen" ); // Screen is defined in nsfw::Assets::init()
        }

        void DeferredApplication::onStep() {

            m_light->update();
            //m_light->direction = normalize( vec3( sin( Window::instance().getTime() ), m_light->direction.y, m_light->direction.z ) );
            m_camera->Update();
            m_soulspear->Update();

            m_geometryPass->prep();
            m_geometryPass->draw( *m_camera, m_soulspear[0] );
            m_geometryPass->draw( *m_camera, m_soulspear[1] );
            m_geometryPass->draw( *m_camera, m_soulspear[2] );
            m_geometryPass->draw( *m_camera, m_soulspear[3] );
            m_geometryPass->post();

            m_pass_GlobalDirectionalLight->prep();
            m_pass_GlobalDirectionalLight->draw( *m_camera, *m_light );
            m_pass_GlobalDirectionalLight->post();

            m_compositePass->prep();
            m_compositePass->draw();
            m_compositePass->post();
        }

        void DeferredApplication::onTerm() {
            delete m_camera;
            delete m_light;
            delete[] m_soulspear;

            delete m_compositePass;
            delete m_geometryPass;
            delete m_pass_GlobalDirectionalLight;
        }
    }
}
