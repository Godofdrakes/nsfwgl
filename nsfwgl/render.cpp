#include "nsfw.h"


// Each Case should set up a uniform using the pass parameters
bool nsfw::RenderPass::setUniform( const char* name, nsfw::UNIFORM::TYPE type, const void* value, unsigned count, bool normalize ) {
    switch ( type ) {
        case nsfw::UNIFORM::FLO1 :
#pragma message ( __WARN__ "Setup float uniform!")
            break;
        case nsfw::UNIFORM::FLO3 :
#pragma message ( __WARN__ "Setup vec3 uniform!")
            break;
        case nsfw::UNIFORM::FLO4 :
#pragma message ( __WARN__ "Setup vec4 uniform!")
            break;
        case nsfw::UNIFORM::MAT4 :
#pragma message ( __WARN__ "Setup mat4 uniform!")
            break;
        case nsfw::UNIFORM::INT1 :
#pragma message ( __WARN__ "Setup integer uniform!")
            break;
        case nsfw::UNIFORM::TEX2 :
#pragma message ( __WARN__ "Setup texture2D uniform!")
            break;
        default :
#pragma message ( __WARN__ "INVALID Uniform type.")
            break;
    }

    return false;
}

// set GL state settings and globally accessible uniforms! Should be called before rendering occurs!
void nsfw::RenderPass::prep() {
#pragma message ( __WARN__ "OVERRIDE: Setup the state of the card for this pass.")
}

// reset the GL state- undo what we did just in case.
void nsfw::RenderPass::post() {
#pragma message ( __WARN__ "OVERRIDE: Clean up the state of the card.")
}
