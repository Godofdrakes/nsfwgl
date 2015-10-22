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

nsfw::GL_HANDLE nsfw::Assets::getVERIFIED( const AssetKey& key ) const {
#ifdef _DEBUG
    if ( !handles.count( key ) ) {
        std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
        assert( false );
        return 0;
    }
#endif
    return handles.at( key );
}

bool nsfw::Assets::setINTERNAL( ASSET::GL_HANDLE_TYPE t, char* name, GL_HANDLE handle ) {
    AssetKey key( t, name );
#ifdef _DEBUG
    if ( handles.count( key ) ) {
        std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
        return false;
    }
    else std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
    handles[key] = handle;
    return true;
}

bool nsfw::Assets::makeVAO( const char* name, const Vertex* verts, unsigned vsize, const unsigned* tris, unsigned tsize ) {
//#pragma message ( __WARN__ "Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!")

    using namespace glbinding;
    using namespace gl;
    unsigned int vao = 0, vbo = 0, ibo = 0;

    ASSET_LOG( GL_HANDLE_TYPE::VAO );
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    ASSET_LOG( GL_HANDLE_TYPE::VBO );
    glGenBuffers( 1, &vbo );
    glBindBuffer( GLenum::GL_ARRAY_BUFFER, vbo );
    glBufferData( GLenum::GL_ARRAY_BUFFER, sizeof( Vertex ) * vsize, verts, GLenum::GL_STATIC_DRAW );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 4, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex ), 0 );

    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 4, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex ), (void*)( sizeof( glm::vec4 ) * 1 ) );

    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2, 4, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex ), (void*)( sizeof( glm::vec4 ) * 2 ) );

    glEnableVertexAttribArray( 3 );
    glVertexAttribPointer( 3, 2, GLenum::GL_FLOAT, GLboolean::GL_FALSE, sizeof( Vertex ), (void*)( sizeof( glm::vec4 ) * 3 ) );

    ASSET_LOG( GL_HANDLE_TYPE::IBO );
    glGenBuffers( 1, &ibo );
    glBindBuffer( GLenum::GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GLenum::GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned ) * tsize, tris, GLenum::GL_STATIC_DRAW );

    ASSET_LOG( GL_HANDLE_TYPE::SIZE );

    handles[AssetKey( GL_HANDLE_TYPE::VAO, name )] = vao;
    handles[AssetKey( GL_HANDLE_TYPE::SIZE, name )] = tsize;

    glBindVertexArray( 0 );
    glBindBuffer( GLenum::GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GLenum::GL_ELEMENT_ARRAY_BUFFER, 0 );

    return vao != 0 && vbo != 0 && ibo != 0 && tsize != 0;
}

bool nsfw::Assets::makeFBO( const char* name, unsigned w, unsigned h, unsigned nTextures, const char* names[], const unsigned depths[] ) {
#pragma message ( __WARN__ "Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.")
    ASSET_LOG(GL_HANDLE_TYPE::FBO);
    using namespace gl;

    GLint maxBuffers; 
    glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &maxBuffers );
    std::vector<GLenum> drawBuffers( maxBuffers );
    for ( int n = 0; n < drawBuffers.size(); ++n ) { drawBuffers[n] = GLenum::GL_COLOR_ATTACHMENT0 + n; }
    
    GLuint frameBuffer = 0;
    glGenFramebuffers( 1, &frameBuffer );
    glBindFramebuffer( GL_FRAMEBUFFER, frameBuffer );

    GLuint colorBufferCount = 0;
    for ( unsigned int n = 0; n < nTextures; ++n ) {
        makeTexture( names[n], w, h, depths[n] );
        GLenum attachment = depths[n] == GL_DEPTH_COMPONENT ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0 + colorBufferCount++;
        glFramebufferTexture2D( GL_FRAMEBUFFER,
                                attachment,
                                GL_TEXTURE_2D,
                                get( TEXTURE, names[n] ),
                                0 );
    }
    glDrawBuffers( colorBufferCount, drawBuffers.data() );

    assert( glCheckFramebufferStatus( GLenum::GL_FRAMEBUFFER ) == GLenum::GL_FRAMEBUFFER_COMPLETE );

    handles[AssetKey( GL_HANDLE_TYPE::FBO, name )] = frameBuffer;
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    return true;
}

bool nsfw::Assets::makeTexture( const char* name, unsigned w, unsigned h, unsigned depth, const char* pixels ) {
//#pragma message ( __WARN__ "Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value." )
    ASSET_LOG( GL_HANDLE_TYPE::TEXTURE );
    using namespace gl;
    GLuint tex = 0;
    glGenTextures( 1, &tex );
    glBindTexture( GLenum::GL_TEXTURE_2D, tex );
    GLenum attachment = (GLenum)depth;
    if ( pixels == nullptr && attachment != GLenum::GL_DEPTH_COMPONENT ) {
        glTexStorage2D( GLenum::GL_TEXTURE_2D, 1, (GLenum)attachment, w, h );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)GL_NEAREST );
    } else {
        glTexImage2D( GLenum::GL_TEXTURE_2D, 0, depth, w, h, 0, (GLenum)attachment, GLenum::GL_UNSIGNED_BYTE, pixels );
        glTexParameteri( GL_TEXTURE_2D, GLenum::GL_TEXTURE_MAG_FILTER, (int)GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GLenum::GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR );
    }
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
    makeTexture( name, w, h, (int)gl::GLenum::GL_RGBA, (char*)pixels );
    stbi_image_free( pixels );
    return true;
}

bool nsfw::Assets::loadShader( const char* name, const char* vpath, const char* fpath ) {
#pragma message ( __WARN__ "Load shader from a file.")
    ASSET_LOG( GL_HANDLE_TYPE::SHADER );
    using namespace gl;
    using namespace std;

    unsigned int program = 0,
        vertexShader = compileShader( gl::GLenum::GL_VERTEX_SHADER, vpath ),
        fragmentShader = compileShader( gl::GLenum::GL_FRAGMENT_SHADER, fpath );

    program = glCreateProgram();
    glAttachShader( program, vertexShader );
    glAttachShader( program, fragmentShader );
    glLinkProgram( program );

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    // Error check
    int success = 0;
    glGetProgramiv( program, GL_LINK_STATUS, &success );
    if ( success == (int)GL_FALSE ) {
        int infoLogSize = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &infoLogSize );
        char* infoLog = new char[infoLogSize];
        glGetProgramInfoLog( program, infoLogSize, 0, infoLog );
        cout << "Error: Failed to link shader program!" << endl;
        cout << infoLog << endl;
        delete[]( infoLog );
        glDeleteProgram( program );
        assert( false );
    }

    handles[AssetKey( GL_HANDLE_TYPE::SHADER, name )] = program;

    return true;
}

unsigned int nsfw::Assets::compileShader( gl::GLenum type, const char* path ) {
    using namespace std;
    using namespace gl;
    assert( fileExists( path ) );

    string source_raw;
    assert( loadText( source_raw, path ) );

    unsigned int handle = glCreateShader( type );
    const char* source_cstr = source_raw.c_str();
    int size = source_raw.size();
    glShaderSource( handle, 1, &source_cstr, &size );
    glCompileShader( handle );

    GLint success = 0;
    glGetShaderiv( handle, GLenum::GL_COMPILE_STATUS, &success );
    if( success == (GLint)GL_FALSE ) {
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
    //name/meshName
    //name/textureName
#pragma message ( __WARN__ "FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n")
    using namespace std;

    assert( fileExists( path ) );

    FBXFile fbx_file;
    if ( !fbx_file.load( path ) ) {
        cout << "Error: Failed to load FBX file \"" << path << "\"" << endl;
        return false;
    }

    for ( unsigned int outer = 0; outer < fbx_file.getMeshCount(); ++outer ) {
        FBXMeshNode* mesh = fbx_file.getMeshByIndex( outer );
        string meshName = name + mesh->m_name;
        vector<Vertex> verts( mesh->m_vertices.size() );
        unsigned int size = verts.size();
        for ( unsigned int inner = 0; inner < size; ++inner ) {
            verts[inner].position = mesh->m_vertices[inner].position;
            verts[inner].normal = mesh->m_vertices[inner].normal;
            verts[inner].tangent = mesh->m_vertices[inner].tangent;
            verts[inner].texCoord = mesh->m_vertices[inner].texCoord1;
        }
        makeVAO( name, verts.data(), verts.size(), mesh->m_indices.data(), mesh->m_indices.size() );
    }

    for ( unsigned int n = 0; n < fbx_file.getTextureCount(); ++n ) {
        FBXTexture* texture = fbx_file.getTextureByIndex( n );
        string textureName = texture->name;
        switch( texture->format ) {
            case 1:
            makeTexture( textureName.c_str(), texture->width, texture->height, (unsigned int)gl::GLenum::GL_R , (char*)texture->data );
                break;
            case 2:
                makeTexture( textureName.c_str(), texture->width, texture->height, (unsigned int)gl::GLenum::GL_RG, (char*)texture->data );
                break;
            case 3:
                makeTexture( textureName.c_str(), texture->width, texture->height, (unsigned int)gl::GLenum::GL_RGB, (char*)texture->data );
                break;
            case 4:
                makeTexture( textureName.c_str(), texture->width, texture->height, (unsigned int)gl::GLenum::GL_RGBA, (char*)texture->data );
                break;
        }
    }

    fbx_file.unload();

    return true;
}

bool nsfw::Assets::loadOBJ( const char* name, const char* path ) {
//#pragma message ( __WARN__ "OBJ file-loading support needed. This function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).")

    using namespace std;
    using namespace tinyobj;
    assert( fileExists( path ) );

    vector<shape_t> shapes;
    vector<material_t> materials;
    string err = LoadObj( shapes, materials, path );

    if( shapes.empty() ) {
        cout << "Error: Failed to load OBJ file \"" << path << "\"" << endl;
        return false;
    }

    for ( unsigned int n = 0; n < shapes.size(); ++n ) {
        using namespace glm;
        string shapeName = name + shapes[n].name;
        vector<Vertex> verts( shapes[n].mesh.positions.size() / 3 );
        for ( unsigned int nn = 0; nn < verts.size(); ++nn ) {
            verts[nn].position = vec4( shapes[n].mesh.positions[nn * 3], shapes[n].mesh.positions[nn * 3 + 1], shapes[n].mesh.positions[nn * 3 + 2], 1.0f );
            verts[nn].normal   = vec4( shapes[n].mesh.normals[nn * 3], shapes[n].mesh.normals[nn * 3 + 1], shapes[n].mesh.normals[nn * 3 + 2], 0.0f );
            verts[nn].tangent  = vec4( verts[nn].normal.y, -verts[nn].normal.x, verts[nn].normal.z, 0.0f );
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
    if ( !good ) { cout << "Error: File \"" << path << "\" could not be opened!" << endl; }
    return good;
}

bool nsfw::Assets::loadText( std::string& fileContent, const char* path ) {
    using namespace std;

    assert( fileExists( path ) );

    string line;
    ifstream file( path );
    fileContent.clear();
    while( getline( file, line ) ) {
        fileContent += line + "\n";
    }

    file.close();
    return true;
}

void nsfw::Assets::init() {
//#pragma message ( __WARN__ "Load up some default assets here if you want.")

    setINTERNAL( FBO, "Screen", 0 );

    makeVAO( "Cube", CubeVerts, 24, CubeTris, 36 );
    makeVAO( "Quad", QuadVerts, 4, QuadTris, 6 );
    /*
	char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGB8, w);
	*/

}

void nsfw::Assets::term() {
//#pragma message ( __WARN__ "" )
    using namespace gl;
    for each ( std::pair<AssetKey,unsigned> k in handles ) {
        switch ( k.first.first ) {
            case VBO :
//#pragma message ( __WARN__ "VBO deletion" )
            case IBO :
//#pragma message ( __WARN__ "IBO deletion" )
            glDeleteBuffers( 1, &k.second );
                break;
            case VAO :
//#pragma message ( __WARN__ "VAO deletion" )
            glDeleteVertexArrays( 1, &k.second );
                break;
            case SHADER :
//#pragma message ( __WARN__ "Shader deletion" )
            glDeleteProgram( k.second );
                break;
            case TEXTURE :
//#pragma message ( __WARN__ "Texture deletion" )
            glDeleteTextures( 1, &k.second );
                break;
            case RBO :
//#pragma message ( __WARN__ "RBO deletion" )
            glDeleteRenderbuffers( 1, &k.second );
                break;
            case FBO :
//#pragma message ( __WARN__ "FBO deletion" )
            glDeleteFramebuffers( 1, &k.second );
                break;
        }
    }
}
