#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass {
public:
    LPassD( const char* shaderName, const char* fboName ) : RenderPass( shaderName, fboName ) {}

    void prep() {
#pragma message ( __WARN__ "glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc...")
    }

    void post() {
#pragma message ( __WARN__ "Unset any gl settings")
    }


    void draw( const Camera& c, const LightD& l ) {
        setUniform( "Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr( c.getProjection() ) );
        setUniform( "View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr( c.getView() ) );

        setUniform( "LightDirection", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr( l.direction ) );
        setUniform( "LightColor", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr( l.color ) );

        setUniform( "TexelScalar", nsfw::UNIFORM::MAT4, glm::value_ptr( nsfw::Window::instance().getTexelAdjustmentMatrix() ) );

        unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>( "Quad" );
        unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>( "Quad" );

#pragma message ( __WARN__ "GL BindVAO/DrawElements with quad size and vao")
    }
};
