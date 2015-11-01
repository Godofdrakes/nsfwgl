#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"

#include "GPass.h"
#include "CPass.h"
#include "RenderPass_GlobalDirectionalLight.h"

using namespace nsfw;

int main() {
    DeferredApplication d;

    d.init();
    d.play();
    d.term();

    system( "pause" );
}


void DeferredApplication::onInit() {
    using namespace gl;
    auto& w = nsfw::Window::instance();
    auto& a = nsfw::Assets::instance();

    // Setup FBOs
    const char* gpassTextureNames[] = { "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
    const GLenum gpassDepths[] = { GL_RGB8, GL_RGB32F, GL_RGB32F, GL_DEPTH_COMPONENT };
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
    m_camera = new Camera;
    m_light = new LightD;
    m_soulspear = new Geometry[2];

    m_camera->lookAt( vec3( 0.f, 2.5f, -5.f ), vec3( 0.f, 2.5f, 0.f ), vec3( 0.f, 1.f, 0.f ) );

    m_light->color = vec3( 1.0f, 1.0f, 1.0f ); // Make sure the light is coming from a direction that makes the world visible to the user
    m_light->direction = normalize( vec3( 0.f, 0.f, 1.f ) );

#pragma message ( "Make sure the following names match the FBX file's output!" )
    m_soulspear[0].mesh = "Soulspear";
    m_soulspear[0].tris = "Soulspear";
    m_soulspear[0].diffuse = "soulspear_diffuse.tga"; // loadFBX will need to name every handle it creates,
    m_soulspear[0].normal = "soulspear_normal.tga"; // These handle names may not be what your loadFBX sets them as!
    m_soulspear[0].specular = "soulspear_specular.tga"; // (Assets will report what the key names are though)
    m_soulspear[0].specPower = 40.0f;
    m_soulspear[0].transform = mat4( 1 );

    m_soulspear[1].mesh = "Soulspear";
    m_soulspear[1].tris = "Soulspear";
    m_soulspear[1].diffuse = "soulspear_diffuse.tga";
    m_soulspear[1].normal = "soulspear_normal.tga";
    m_soulspear[1].specular = "soulspear_specular.tga";
    m_soulspear[1].specPower = 40.0f;
    m_soulspear[1].transform = translate( 5.f, 0.f, 0.f );

    m_geometryPass = new GPass( "GeometryPassPhong", "GeometryPass" );
    m_pass_GlobalDirectionalLight = new RenderPass_GlobalDirectionalLight( "LightPassDirectional", "LightPass" );
    m_compositePass = new CPass( "CompPass", "Screen" ); // Screen is defined in nsfw::Assets::init()
}

void DeferredApplication::onStep() {
    m_light->update();
    m_light->direction = normalize( vec3( sin( Window::instance().getTime() ) * 5.f, m_light->direction.y, m_light->direction.z ) );
    m_camera->update();
    m_soulspear->update();

    m_geometryPass->prep();
    m_geometryPass->draw( *m_camera, m_soulspear[0] );
    m_geometryPass->draw( *m_camera, m_soulspear[1] );
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
