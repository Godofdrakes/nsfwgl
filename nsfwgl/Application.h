#pragma once
#include "nsfw.h"
#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>

namespace nsfw {

    class Application {
    public:
        Application() {}

        virtual ~Application() {}

        virtual void onInit() {
#pragma message ( "Override this and load everything" )
        };

        virtual void onTerm() {
#pragma message ( "Override this and unload everything" )
        };

        virtual void onPlay() {
#pragma message ( "Override this and start the game" )
        };

        virtual void onStep() {
#pragma message ( "Override this and update and draw" )
        };

        void init( int width = 800, int height = 600, const char* name = "Not Suited For Work" ) {
            Window::instance().init( width, height );

            // Init glbinding
            glbinding::Binding::initialize();
            assert( gl::glGetError() == gl::GLenum::GL_NO_ERROR );

            gl::GLint major = 0, minor = 0;
            gl::glGetIntegerv( gl::GLenum::GL_MAJOR_VERSION, &major );
            gl::glGetIntegerv( gl::GLenum::GL_MINOR_VERSION, &minor );
            std::cout << "OpenGL Version: " << major << "." << minor << std::endl;

#ifdef _DEBUG
            // glbinding provides the ability to set callback functions for any and all OpenGL function calls
            // The folowing is the equivilant of calling glGetError() after EVERY SINGLE OpenGL call.
            // Performance hit? Probably. Do I care? Not right now, no.
            glbinding::setCallbackMaskExcept( glbinding::CallbackMask::After, { "glGetError" } );
            glbinding::setAfterCallback( []( const glbinding::FunctionCall&) {
                                            using namespace gl;
                                            GLenum error = glGetError();
                                            if ( error != GL_NO_ERROR ) {
                                                std::cout << "ERROR: " << std::hex << error << std::endl;
                                                assert( false );
                                            }
                                        } );

            glbinding::Binding::LinkProgram.setAfterCallback( []( const gl::GLuint program ) {
                                                                 using namespace gl;
                                                                 GLint success = 0;
                                                                 glGetProgramiv( program, GL_LINK_STATUS, &success );
                                                                 if ( success == 0 ) {
                                                                     using namespace std;
                                                                     GLint infoLogSize = 0;
                                                                     glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogSize );
                                                                     GLchar* infoLog = new char[infoLogSize];
                                                                     glGetProgramInfoLog( program, infoLogSize, 0, infoLog );
                                                                     cout << "Error: Failed to link shader program!" << endl;
                                                                     cout << infoLog << endl;
                                                                     delete[](infoLog);
                                                                     glDeleteProgram( program ); // CLEAN UP YOUR SHIT
                                                                     assert(false);
                                                                 }
                                                             }
                                                            );
#endif

            Assets::instance().init(); // Initialize the asset manager
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

