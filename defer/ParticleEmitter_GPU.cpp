#include "ParticleEmitter_GPU.h"
#include "../nsfwgl/Assets.h"
#include <glbinding/gl/gl.h>
#include <string>
#include <glm/ext.hpp>

namespace nsfw {
    namespace particles {

        unsigned int ParticleEmitter_GPU::m_updateShader = 0,
                ParticleEmitter_GPU::m_drawShader = 0;

        void ParticleEmitter_GPU::CreateParticleBuffers() {
            using namespace gl;

            // Create the particle buffers on the GPU
            glGenVertexArrays( 2, m_vao.data() );
            glGenBuffers( 2, m_vbo.data() );

            // Define the particle buffers on the GPU
            for ( unsigned n = 0; n < 2; ++n ) {
                std::vector<Particle_GPU> bufferInit( MAX_PARTICLES );

                glBindVertexArray( m_vao[n] );
                glBindBuffer( GL_ARRAY_BUFFER, m_vbo[n] );
                glBufferData( GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle_GPU), bufferInit.data(), GL_STREAM_DRAW );

                glEnableVertexAttribArray( 0 ); // position
                glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle_GPU), 0 );

                glEnableVertexAttribArray( 1 ); // velocity
                glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle_GPU), ( void* )(sizeof(glm::vec3)) );

                glEnableVertexAttribArray( 2 ); // lifeTime
                glVertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle_GPU), ( void* )(sizeof(glm::vec3) * 2) );

                glEnableVertexAttribArray( 3 ); // lifeSpan
                glVertexAttribPointer( 3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle_GPU), ( void* )(sizeof(glm::vec3) * 2 + sizeof(float)) );

                glBindVertexArray( 0 );
                glBindBuffer( GL_ARRAY_BUFFER, 0 );
            }
        }

        void ParticleEmitter_GPU::DestroyParticleBuffers() {
            using namespace gl;
            glDeleteVertexArrays( 2, m_vao.data() );
            m_vao = { 0, 0 };
            glDeleteBuffers( 2, m_vbo.data() );
            m_vbo = { 0, 0 };
        }

        void ParticleEmitter_GPU::CreateUpdateShader() {
            using namespace gl;
            using namespace std;
            if ( m_updateShader != 0 ) {
                return;
            }
            GLuint shader = Assets::instance().compileShader( GL_VERTEX_SHADER, "./Assets/Shaders/Particles/Update.glsl" );
            GLuint program = glCreateProgram();
            glAttachShader( program, shader );

            const char* returnValues[] = { "position", "velocity", "lifeTime", "lifeSpan" };

            glTransformFeedbackVaryings( program,
                                         4,
                                         returnValues,
                                         GL_INTERLEAVED_ATTRIBS );

            glLinkProgram( program ); // Error checking is in callback
            glDeleteShader( shader );

            m_updateShader = program;

        }

        void ParticleEmitter_GPU::CreateDrawShader() {
            using namespace gl;
            using namespace std;
            if ( m_drawShader != 0 ) {
                return;
            }

            GLuint vertexShader = Assets::instance().compileShader( GL_VERTEX_SHADER, "./Assets/Shaders/Particles/Vertex.glsl" );
            GLuint fragmentShader = Assets::instance().compileShader( GL_FRAGMENT_SHADER, "./Assets/Shaders/Particles/Fragment.glsl" );
            GLuint geometryShader = Assets::instance().compileShader( GL_GEOMETRY_SHADER, "./Assets/Shaders/Particles/Geometry.glsl" );

            GLuint program = glCreateProgram();
            glAttachShader( program, vertexShader );
            glAttachShader( program, fragmentShader );
            glAttachShader( program, geometryShader );
            glLinkProgram( program );

            glDeleteShader( vertexShader );
            glDeleteShader( fragmentShader );
            glDeleteShader( geometryShader );

            m_drawShader = program;
        }

        void ParticleEmitter_GPU::ShaderUpdate( float deltaTime, float time ) {
            using namespace gl;

            GLuint inputBuffer = m_activeBuffer, outputBuffer = 1 - m_activeBuffer;

            glUseProgram( m_updateShader );

            glUniform1f( glGetUniformLocation( m_updateShader, "lifeSpan_MIN" ), m_settings.lifeSpan_MIN );
            glUniform1f( glGetUniformLocation( m_updateShader, "lifeSpan_MAX" ), m_settings.lifeSpan_MAX );
            glUniform1f( glGetUniformLocation( m_updateShader, "time" ), time );
            glUniform1f( glGetUniformLocation( m_updateShader, "deltaTime" ), deltaTime );
            glUniform3fv( glGetUniformLocation( m_updateShader, "emitterPosition" ), 1, glm::value_ptr( Position ) );

            glBindVertexArray( m_vao[inputBuffer] );

            glEnable( GL_RASTERIZER_DISCARD );
            glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[outputBuffer] );
            glBeginTransformFeedback( GL_POINTS );

            glDrawArrays( GL_POINTS, 0, MAX_PARTICLES );

            glEndTransformFeedback();
            glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0 );
            glDisable( GL_RASTERIZER_DISCARD );

            glUseProgram( 0 );
            m_activeBuffer = outputBuffer;
        }

        void ParticleEmitter_GPU::ShaderDraw( const glm::mat4 cametaTransform, const glm::mat4 projectionView ) {
            using namespace gl;

            glUseProgram( m_drawShader );
            glUniform1f( glGetUniformLocation( m_drawShader, "size_START" ), m_settings.size_START );
            glUniform1f( glGetUniformLocation( m_drawShader, "size_END" ), m_settings.size_END );
            glUniform4fv( glGetUniformLocation( m_drawShader, "color_START" ), 1, glm::value_ptr( m_settings.color_START ) );
            glUniform4fv( glGetUniformLocation( m_drawShader, "color_END" ), 1, glm::value_ptr( m_settings.color_END ) );
            glUniformMatrix4fv( glGetUniformLocation( m_drawShader, "cametaTransform" ), 1, GL_FALSE, glm::value_ptr( cametaTransform ) );
            glUniformMatrix4fv( glGetUniformLocation( m_drawShader, "projectionView" ), 1, GL_FALSE, glm::value_ptr( projectionView ) );

            glBindVertexArray( m_vao[m_activeBuffer] );
            glDrawArrays( GL_POINTS, 0, MAX_PARTICLES );

            glUseProgram( 0 );
        }
    }
}

