#include "nsfw.h"
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <glbinding/gl/gl.h>
#include <stb/stb_image.h>
#include <tinyobj/tiny_obj_loader.h>
#include <FBXFile/FBXFile.h>

using namespace nsfw::ASSET;

const char* nsfw::TYPE_NAMES[eSIZE + 1] = { "NONE","vao","ibo","vbo","tri-size","fbo","rbo","texture","shader","SIZE" };

#ifdef _DEBUG
#define ASSET_LOG(type) do { std::cerr << "Trying to create key: <" << TYPE_NAMES[type] << ">" << name << std::endl; } while(0)
#else 
#define ASSET_LOG(type) do {} while (0)
#endif

gl::GLuint nsfw::Assets::getVERIFIED( const AssetKey& key ) const {
    if ( !handles.count( key ) ) {
#ifdef _DEBUG
        std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
        assert( false );
#endif
        return 0;
    }
    return handles.at( key );
}

bool nsfw::Assets::setINTERNAL( ASSET::GL_HANDLE_TYPE t, const char* name, gl::GLuint handle ) {
    AssetKey key( t, name );
#ifdef _DEBUG
    if ( handles.count( key ) ) {
        std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
        return false;
    }
    std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
    handles[key] = handle;
    return true;
}

bool nsfw::Assets::makeVAO( const char* name, const Vertex* verts, unsigned vsize, const unsigned* tris, unsigned tsize ) {
    using namespace glbinding;
    using namespace gl;
    GLuint glHandle_vao = 0, glHandle_vbo = 0, glHandle_ibo = 0;

    assert( tsize != 0 );
#ifndef _DEBUG
    if ( tsize == 0 ) {
        std::cout << "Error: makeVAO, tsize cannot be 0" << std::endl;
        return false;
    }
#endif

    glGenVertexArrays( 1, &glHandle_vao );
    glBindVertexArray( glHandle_vao );
    assert( glHandle_vao != 0 );
#ifndef _DEBUG
    if ( glHandle_vao == 0 ) {
        std::cout << "Error: makeVAO, failed to generate glHandle_vao" << std::endl;
        return false;
    }
#endif

    glGenBuffers( 1, &glHandle_vbo );
    assert( glHandle_vbo != 0 );
#ifndef _DEBUG
    if ( glHandle_vbo == 0 ) {
        std::cout << "Error: makeVAO, failed to generate glHandle_vbo" << std::endl;
        glDeleteBuffers( 1, &glHandle_vao );
        return false;
    }
#endif
    glBindBuffer( GLenum::GL_ARRAY_BUFFER, glHandle_vbo );
    glBufferData( GLenum::GL_ARRAY_BUFFER, sizeof( Vertex) * vsize, verts, GLenum::GL_STATIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex), 0 );
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 4, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex), ( void* )(sizeof( glm::vec4) * 1) );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 4, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex), ( void* )(sizeof( glm::vec4) * 2) );
    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 3, 2, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex), ( void* )(sizeof( glm::vec4) * 3) );

    glGenBuffers( 1, &glHandle_ibo );
    assert( glHandle_ibo != 0 );
#ifndef _DEBUG
    if ( glHandle_ibo == 0 ) {
        std::cout << "Error: makeVAO, failed to generate glHandle_ibo" << std::endl;
        glDeleteBuffers( 1, &glHandle_vbo );
        glDeleteBuffers( 1, &glHandle_vao );
        return false;
    }
#endif
    glBindBuffer( GLenum::GL_ELEMENT_ARRAY_BUFFER, glHandle_ibo );
    glBufferData( GLenum::GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned) * tsize, tris, GLenum::GL_STATIC_DRAW );

    ASSET_LOG( GL_HANDLE_TYPE::VAO );
    handles[AssetKey( VAO, name )] = glHandle_vao;
    ASSET_LOG( GL_HANDLE_TYPE::VBO );
    handles[AssetKey( VBO, name )] = glHandle_vbo;
    ASSET_LOG( GL_HANDLE_TYPE::IBO );
    handles[AssetKey( IBO, name )] = glHandle_ibo;
    ASSET_LOG( GL_HANDLE_TYPE::SIZE );
    handles[AssetKey( SIZE, name )] = tsize;

    glBindVertexArray( 0 );
    glBindBuffer( GLenum::GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GLenum::GL_ELEMENT_ARRAY_BUFFER, 0 );

    return true;
}

bool nsfw::Assets::makeFBO( const char* name, unsigned w, unsigned h, unsigned nTextures, const char* names[], const gl::GLenum depths[] ) {
    using namespace gl;

    /*
        The idea here is we make a vector representing the maximum number of color attachments available.
        Then we just pass this into glDrawBuffers(), with the prameters being ( theNumberOfBuffersWeActuallyUse, colorBufferVector.data() )
        This also means (for those who care) we can assert if we try to use more color buffers than are available.
    */
    GLint maxAvailableColorBuffers = 0;
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &maxAvailableColorBuffers );
    assert( maxAvailableColorBuffers >= 8 );
#ifndef _DEBUG
    if ( maxAvailableColorBuffers < 8 ) {
        std::cout << "Error: makeFBO, maxAvailableColorBuffers < 8" << std::endl;
        return false;
    }
#endif
    std::vector<GLenum> colorBufferArray( maxAvailableColorBuffers );
    for ( GLint n = 0; n < maxAvailableColorBuffers; ++n ) {
        colorBufferArray[n] = GLenum::GL_COLOR_ATTACHMENT0 + n;
    }

    GLuint glHandle_frameBuffer = 0;
    glGenFramebuffers( 1, &glHandle_frameBuffer );
    assert( glHandle_frameBuffer != 0 );
#ifndef _DEBUG
    if ( glHandle_frameBuffer == 0 ) {
        std::cout << "Error: makeFBO, failed to generate glHandle_frameBuffer" << std::endl;
        return false;
    }
#endif
    glBindFramebuffer( GL_FRAMEBUFFER, glHandle_frameBuffer );

    GLuint colorBufferCount = 0; // How many of the depths are color buffers?
    for ( unsigned int n = 0; n < nTextures; ++n ) {
        makeTexture( names[n], w, h, depths[n] );
        GLenum attachment = depths[n] == GL_DEPTH_COMPONENT ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + colorBufferCount++; // colorBufferCount++ !!!!!! NOT ++colorBufferCount.
        glFramebufferTexture2D( GL_FRAMEBUFFER,
                                attachment,
                                GL_TEXTURE_2D,
                                get( TEXTURE, names[n] ),
                                0 );
    }
    glDrawBuffers( colorBufferCount, colorBufferArray.data() );

    assert( glCheckFramebufferStatus( GLenum::GL_FRAMEBUFFER ) == GLenum::GL_FRAMEBUFFER_COMPLETE );

    ASSET_LOG( GL_HANDLE_TYPE::FBO );
    setINTERNAL( FBO, name, glHandle_frameBuffer );

    glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Clean up after yourself
    return true;
}

bool nsfw::Assets::makeTexture( const char* name, unsigned w, unsigned h, gl::GLenum depth, const char* pixels ) {
    using namespace gl;
    GLuint tex = 0;
    glGenTextures( 1, &tex );
    glBindTexture( GLenum::GL_TEXTURE_2D, tex );
    if ( pixels == nullptr && depth != GLenum::GL_DEPTH_COMPONENT ) {
        glTexStorage2D( GLenum::GL_TEXTURE_2D, 1, depth, w, h );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ( int )GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ( int )GL_NEAREST );
    }
    else {
        glTexImage2D( GLenum::GL_TEXTURE_2D, 0, ( unsigned int )depth, w, h, 0, depth, GLenum::GL_UNSIGNED_BYTE, pixels );
        glTexParameteri( GL_TEXTURE_2D, GLenum::GL_TEXTURE_MAG_FILTER, ( int )GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GLenum::GL_TEXTURE_MIN_FILTER, ( int )GL_LINEAR );
    }

    ASSET_LOG( GL_HANDLE_TYPE::TEXTURE );
    handles[AssetKey( GL_HANDLE_TYPE::TEXTURE, name )] = tex;

    glBindTexture( GLenum::GL_TEXTURE_2D, 0 );
    return true;
}

bool nsfw::Assets::loadTexture( const char* name, const char* path ) {
    using namespace std;
    assert( fileExists( path ) );
    int w = 0, h = 0, format = 0;
    // technically format value is useless. The if 5th parameter is not 0 then it will convert the loaded data to that format
    unsigned char* pixels = stbi_load( path, &w, &h, &format, STBI_rgb_alpha );
    makeTexture( name, w, h, gl::GLenum::GL_RGBA, ( char* )pixels );
    stbi_image_free( pixels );
    return true;
}

bool nsfw::Assets::loadShader( const char* name, const char* vpath, const char* fpath ) {
    using namespace gl;

    GLuint program = glCreateProgram(),
            vertexShader = compileShader( GLenum::GL_VERTEX_SHADER, vpath ),
            fragmentShader = compileShader( GLenum::GL_FRAGMENT_SHADER, fpath );

    glAttachShader( program, vertexShader );
    glAttachShader( program, fragmentShader );
    glLinkProgram( program );

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    // Error check
    GLint success = 0;
    glGetProgramiv( program, GL_LINK_STATUS, &success );
    if ( success == ( GLint )GL_FALSE ) {
        using namespace std;
        GLint infoLogSize = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogSize );
        GLchar* infoLog = new char[infoLogSize];
        glGetProgramInfoLog( program, infoLogSize, 0, infoLog );
        cout << "Error: Failed to link shader program!" << endl;
        cout << infoLog << endl;
        delete[](infoLog);
        glDeleteProgram( program ); // CLEAN UP YOUR SHIT
        assert( false );
    }

    ASSET_LOG( GL_HANDLE_TYPE::SHADER );
    handles[AssetKey( GL_HANDLE_TYPE::SHADER, name )] = program;

    return true;
}

unsigned int nsfw::Assets::compileShader( gl::GLenum type, const char* path ) {
    using namespace std;
    using namespace gl;

    string source_raw;
    assert( loadText( source_raw, path ) ); // LoadText checks if the file exists.

    GLuint handle = glCreateShader( type ); {
        const GLchar* source_cstr = source_raw.c_str();
        int size = source_raw.size();
        glShaderSource( handle, 1, &source_cstr, &size );
        glCompileShader( handle );
    }

    // Shader error check
    GLint success = 0;
    glGetShaderiv( handle, GLenum::GL_COMPILE_STATUS, &success );
    if ( success == ( GLint )GL_FALSE ) {
        GLint log_length = 0;
        glGetShaderiv( handle, GL_INFO_LOG_LENGTH, &log_length );

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog( log_length );
        glGetShaderInfoLog( handle, log_length, &log_length, &errorLog[0] );

        cout << errorLog.data() << endl;

        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader( handle );
        assert( false );
        return 0;
    }

    return handle;
}

bool nsfw::Assets::loadFBX( const char* name, const char* path ) {
    using namespace std;
    assert( fileExists( path ) ); // Check the file exists

    FBXFile fbx_file;
    if ( !fbx_file.load( path ) ) { // Check FBXLoader can load the file
        cout << "Error: Failed to load FBX file \"" << path << "\"" << endl;
        return false;
    }

    for ( unsigned int outer = 0; outer < fbx_file.getMeshCount(); ++outer ) { // For each mesh
        FBXMeshNode* mesh = fbx_file.getMeshByIndex( outer );
        string meshName = name + mesh->m_name;
        vector<Vertex> verts( mesh->m_vertices.size() );
        unsigned int size = verts.size();
        for ( unsigned int inner = 0; inner < size; ++inner ) { // For each vertex in the mesh
            FBXVertex* vertex = &mesh->m_vertices[inner];
            verts[inner].position = vertex->position;
            verts[inner].normal = vertex->normal;
            verts[inner].tangent = vertex->tangent;
            verts[inner].texCoord = vertex->texCoord1;
        }
        makeVAO( name, verts.data(), verts.size(), mesh->m_indices.data(), mesh->m_indices.size() );
    }

    for ( unsigned int n = 0; n < fbx_file.getTextureCount(); ++n ) { // For each texture
        FBXTexture* texture = fbx_file.getTextureByIndex( n );
        string textureName = texture->name;
        switch ( texture->format ) {
            case 1:
                makeTexture( textureName.c_str(), texture->width, texture->height, gl::GLenum::GL_R, ( char* )texture->data );
                break;
            case 2:
                makeTexture( textureName.c_str(), texture->width, texture->height, gl::GLenum::GL_RG, ( char* )texture->data );
                break;
            case 3:
                makeTexture( textureName.c_str(), texture->width, texture->height, gl::GLenum::GL_RGB, ( char* )texture->data );
                break;
            case 4:
                makeTexture( textureName.c_str(), texture->width, texture->height, gl::GLenum::GL_RGBA, ( char* )texture->data );
                break;
            default:
                assert( false && "Invalid texture depth found while loading FBX object." );
                break;
        }
    }

    fbx_file.unload(); // Clean up, clean up, everybody do your stuff

    return true;
}

bool nsfw::Assets::loadOBJ( const char* name, const char* path ) {
    using namespace std;
    using namespace tinyobj;
    assert( fileExists( path ) );

    vector<shape_t> shapes;
    vector<material_t> materials;
    string err = LoadObj( shapes, materials, path );

    if ( shapes.empty() ) {
        cout << "Error: Failed to load OBJ file \"" << path << "\"" << endl;
        return false;
    }

    for ( unsigned int n = 0; n < shapes.size(); ++n ) {
        using namespace glm;
        string shapeName = name + shapes[n].name;
        vector<Vertex> verts( shapes[n].mesh.positions.size() / 3 );
        for ( unsigned int nn = 0; nn < verts.size(); ++nn ) {
            verts[nn].position = vec4( shapes[n].mesh.positions[nn * 3], shapes[n].mesh.positions[nn * 3 + 1], shapes[n].mesh.positions[nn * 3 + 2], 1.0f );
            verts[nn].normal = vec4( shapes[n].mesh.normals[nn * 3], shapes[n].mesh.normals[nn * 3 + 1], shapes[n].mesh.normals[nn * 3 + 2], 0.0f );
            verts[nn].tangent = vec4( verts[nn].normal.y, -verts[nn].normal.x, verts[nn].normal.z, 0.0f );
            verts[nn].texCoord = vec2( shapes[n].mesh.texcoords[nn * 2], shapes[n].mesh.texcoords[nn * 2 + 1] );
        }
        makeVAO( shapeName.c_str(), verts.data(), verts.size(), shapes[n].mesh.indices.data(), shapes[n].mesh.indices.size() );
    }

    return true;
}

bool nsfw::Assets::fileExists( const char* path ) {
    using namespace std;
    ifstream file( path );
    bool good = file.good();
    file.close();
    if ( !good ) {
        cout << "Error: File \"" << path << "\" could not be opened!" << endl;
    }
    return good;
}

bool nsfw::Assets::loadText( std::string& fileContent, const char* path ) {
    using namespace std;
    assert( fileExists( path ) );
    string line;
    ifstream file( path );
    fileContent.clear();
    while ( getline( file, line ) ) {
        fileContent += line + "\n";
    } // REMEMBER TO ADD NEW LINES. GETLINE STRIPPS LINE ENDINGS
    file.close();
    return true;
}

void nsfw::Assets::init() {
    setINTERNAL( FBO, "Screen", 0 );
    makeVAO( "Cube", CubeVerts, 24, CubeTris, 36 );
    makeVAO( "Quad", QuadVerts, 4, QuadTris, 6 );

    // Fallback assets
    const std::vector<char> white( { ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255, ( char )255 } );
    const std::vector<char> blue( { ( char )127, ( char )127, ( char )255, ( char )127, ( char )127, ( char )127, ( char )255, ( char )127, ( char )127, ( char )127, ( char )255, ( char )127, ( char )127, ( char )127, ( char )255, ( char )127 } );
    const std::vector<char> black( { ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0, ( char )0 } );
    makeTexture( "Fallback_Diffuse", 2, 2, gl::GLenum::GL_RGBA, white.data() );
    makeTexture( "Fallback_Normal", 2, 2, gl::GLenum::GL_RGBA, blue.data() );
    makeTexture( "Fallback_Specular", 2, 2, gl::GLenum::GL_RGBA, black.data() );
}

void nsfw::Assets::term() {
#pragma message ( "CLEAN UP AFTER YOURSELF YOUNG MAN" )
    using namespace gl;
    for each ( std::pair<AssetKey, unsigned> k in handles ) {
        switch ( k.first.first ) {
            case VBO:
                glDeleteBuffers( 1, &k.second );
            case IBO:
                glDeleteBuffers( 1, &k.second );
                break;
            case VAO:
                glDeleteVertexArrays( 1, &k.second );
                break;
            case SHADER:
                glDeleteProgram( k.second );
                break;
            case TEXTURE:
                glDeleteTextures( 1, &k.second );
                break;
            case RBO:
                glDeleteRenderbuffers( 1, &k.second );
                break;
            case FBO:
                glDeleteFramebuffers( 1, &k.second );
                break;
        }
    }
}
