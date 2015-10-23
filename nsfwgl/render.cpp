#include "nsfw.h"


// Each Case should set up a uniform using the pass parameters
bool nsfw::RenderPass::setUniform( const char* name, nsfw::UNIFORM::TYPE type, const void* value, unsigned count, bool normalize ) {
    using namespace gl;
    int location = glGetUniformLocation( *shader, name );
    assert( location != -1 && "Uniform does not exist." );
    assert( value != nullptr && "Value must not be null" );
    switch ( type ) {
        case nsfw::UNIFORM::FLO1:
            glUniform1fv( location, count, ( float* )value );
            break;
        case nsfw::UNIFORM::FLO3:
            glUniform3fv( location, count, ( float* )value );
            break;
        case nsfw::UNIFORM::FLO4:
            glUniform4fv( location, count, ( float* )value );
            break;
        case nsfw::UNIFORM::MAT4:
            glUniformMatrix4fv( location, count, GLboolean::GL_FALSE, (float*)value );
            break;
        case nsfw::UNIFORM::INT1:
            glUniform1iv( location, count, ( int* )value );
            break;
        case nsfw::UNIFORM::TEX2:
            glUniform1i( location, count );
            glActiveTexture( GL_TEXTURE0 + count );
            glBindTexture( GL_TEXTURE_2D, *(GLuint*)value );
            break;
        default:
            assert( false && "INVALID Uniform type." );
            break;
    }

    return false;
}

// set GL state settings and globally accessible uniforms! Should be called before rendering occurs!
void nsfw::RenderPass::prep() {
    #pragma message ( "Override and setup the card for this render")
}

// reset the GL state- undo what we did just in case.
void nsfw::RenderPass::post() {
    #pragma message ( "Override and undo all sets in prep")
}
