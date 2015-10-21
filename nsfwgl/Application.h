#pragma once
#include "nsfw.h"
#include <glbinding/Version.h>
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

namespace nsfw {

    class Application {
    public:
        virtual void onInit() {
#pragma message ( __WARN__ "OVERRIDE: Load everything here!" )
        };

        virtual void onTerm() {
#pragma message ( __WARN__ "OVERRIDE: Free your data!" )
        };

        virtual void onPlay() {
#pragma message ( __WARN__ "OVERRIDE: Setup/reset the game state (or do nothing)!" )
        };

        virtual void onStep() {
#pragma message ( __WARN__ "OVERRIDE: Render Passes should draw, objects should update!" )
        };

        void init( int width = 800, int height = 600, const char* name = "Not Suited For Work" ) {
            using namespace glbinding;
            Window::instance().init( width, height );

            // Init glBinding
            Binding::initialize();
            gl::GLenum error = gl::glGetError();
            std::cout << "error: " << std::hex << error << std::endl;
            assert( error == gl::GLenum::GL_NO_ERROR );

            int major = 0, minor = 0;
            gl::glGetIntegerv( gl::GLenum::GL_MAJOR_VERSION, &major );
            gl::glGetIntegerv( gl::GLenum::GL_MINOR_VERSION, &minor );
            std::cout << "OpenGL Version: " << major << "." << minor << std::endl;

            setCallbackMaskExcept( CallbackMask::After, { "glGetError" } );
            setAfterCallback( []( const FunctionCall & ) {
                using namespace gl;
                GLenum error = glGetError();
                if ( error != GL_NO_ERROR ) {
                    std::cout << "ERROR: " << std::hex << error << std::endl;
                    assert( false );
                }
            } );

            Assets::instance().init();
            onInit();
        }

        void term() {
            onTerm();
            Assets::instance().term();
            Window::instance().term();
        }

        void play() {
            onPlay();

            do {
                step();
            }
            while ( !Window::instance().getShouldClose() );
        }

        void step() {
            Window::instance().step();
            onStep();
        }
    };
}
