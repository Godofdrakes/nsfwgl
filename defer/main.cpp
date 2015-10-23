#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"

#include "GPass.h"
#include "CPass.h"
#include "LPassD.h"

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
    a.loadShader( "GeometryPassPhong", "./Assets/Shaders/Geometry/Vertex.glsl", "./Assets/Shaders/Geometry/Fragment.glsl" );
    a.loadShader( "LightPassDirectional", "./Assets/Shaders/Lighting/Vertex.glsl", "./Assets/Shaders/Lighting/Fragment.glsl" );
    a.loadShader( "CompPass", "./Assets/Shaders/Composite/Vertex.glsl", "./Assets/Shaders/Composite/Fragment.glsl" );

    // Load any other textures and geometry we want to use
    a.loadFBX( "Soulspear", "./Assets/FBX/SoulSpear/soulspear.fbx" );
}

void DeferredApplication::onPlay() {
    m_camera = new Camera;
    m_light = new LightD;
    m_soulspear = new Geometry;

    m_camera->lookAt( glm::vec3( 5 ), glm::vec3( 0, 1, 0 ), glm::vec3( 0, 1, 0 ) );

    m_light->color = glm::vec3( 1, 0, 1 ); // Make sure the light is coming from a direction that makes the world visible to the user
    m_light->direction = glm::normalize( glm::vec3( 1, 0, 1 ) );

#pragma message ( "Make sure the following names match the FBX file's output!" )
    m_soulspear->mesh = "Soulspear";
    m_soulspear->tris = "Soulspear";
    m_soulspear->diffuse = "soulspear_diffuse.tga"; // loadFBX will need to name every handle it creates,
    m_soulspear->normal = "soulspear_normal.tga"; // These handle names may not be what your loadFBX sets 
    m_soulspear->specular = "soulspear_specular.tga"; // them as! (Assets will report what the key names are though)
    m_soulspear->specPower = 40.0f;
    m_soulspear->transform = mat4( 1 );

    m_geometryPass = new GPass( "GeometryPassPhong", "GeometryPass" );
    m_directionalLightPass = new LPassD( "LightPassDirectional", "LightPass" );
    m_compositePass = new CPass( "CompPass", "Screen" ); // Screen is defined in nsfw::Assets::init()
}

void DeferredApplication::onStep() {
    m_light->update();
    m_camera->update();
    m_soulspear->update();

    m_geometryPass->prep();
    m_geometryPass->draw( *m_camera, *m_soulspear );
    m_geometryPass->post();

    m_directionalLightPass->prep();
    m_directionalLightPass->draw( *m_camera, *m_light );
    m_directionalLightPass->post();

    m_compositePass->prep();
    m_compositePass->draw();
    m_compositePass->post();
}

void DeferredApplication::onTerm() {
    delete m_camera;
    delete m_light;
    delete m_soulspear;

    delete m_compositePass;
    delete m_geometryPass;
    delete m_directionalLightPass;
}
