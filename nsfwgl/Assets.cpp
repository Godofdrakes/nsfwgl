#include "nsfw.h"
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
    unsigned int size = tsize;

    ASSET_LOG( GL_HANDLE_TYPE::VAO );
    glGenVertexArrays( 1, &vao );
    handles[AssetKey( GL_HANDLE_TYPE::VAO, name )] = vao;
    glBindVertexArray( vao );

    ASSET_LOG( GL_HANDLE_TYPE::VBO );
    glGenBuffers( 1, &vbo );
    handles[AssetKey( GL_HANDLE_TYPE::VBO, name )] = vbo;
    glBindBuffer( GLenum::GL_ARRAY_BUFFER, vbo );
    glBufferData( GLenum::GL_ARRAY_BUFFER, sizeof( Vertex ) * vsize, verts, GLenum::GL_STATIC_DRAW );

    ASSET_LOG( GL_HANDLE_TYPE::IBO );
    glGenBuffers( 1, &ibo );
    handles[AssetKey( GL_HANDLE_TYPE::IBO, name )] = ibo;
    glBindBuffer( GLenum::GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData( GLenum::GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned ) * tsize, tris, GLenum::GL_STATIC_DRAW );

    ASSET_LOG( GL_HANDLE_TYPE::SIZE );
    handles[AssetKey( GL_HANDLE_TYPE::SIZE, name )] = tsize;

    glBindVertexArray( 0 );
    glBindBuffer( GLenum::GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GLenum::GL_ELEMENT_ARRAY_BUFFER, 0 );

    return vao != 0 && vbo != 0 && ibo != 0 && size != 0;
}

bool nsfw::Assets::makeFBO( const char* name, unsigned w, unsigned h, unsigned nTextures, const char* names[], const unsigned depths[] ) {
//#pragma message ( __WARN__ "Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.")
    ASSET_LOG(GL_HANDLE_TYPE::FBO);
    using namespace gl;
    unsigned int fbo = 0, depth = 0;

    glGenBuffers( 1, &fbo );
    handles[AssetKey( GL_HANDLE_TYPE::FBO, name )] = fbo;
    glBindFramebuffer( GLenum::GL_FRAMEBUFFER, fbo );

    std::vector<GLenum> drawBuffers;

    for ( unsigned int n = 0; n < nTextures && GLenum::GL_COLOR_ATTACHMENT0 + n <= GLenum::GL_COLOR_ATTACHMENT31; ++n ) {
        makeTexture( names[n], w, h, depths[n] );
        glFramebufferTexture( GLenum::GL_FRAMEBUFFER, GLenum::GL_COLOR_ATTACHMENT0 + n, handles[AssetKey( GL_HANDLE_TYPE::TEXTURE, names[n] )], 0 );
        drawBuffers.push_back( GL_COLOR_ATTACHMENT0 + n );
    }

    glGenRenderbuffers( 1, &depth );
    handles[AssetKey( GL_HANDLE_TYPE::RBO, name )] = depth;
    makeTexture( name, w, h, (unsigned int)GLenum::GL_RGB8 );
    glFramebufferTexture( GLenum::GL_FRAMEBUFFER, GLenum::GL_DEPTH_ATTACHMENT, depth, 0 );

    glDrawBuffers( drawBuffers.size(), drawBuffers.data() );

    GLenum status = glCheckFramebufferStatus( GLenum::GL_FRAMEBUFFER );
    glBindFramebuffer( GLenum::GL_FRAMEBUFFER, 0 );
    if( status != GLenum::GL_FRAMEBUFFER_COMPLETE ) {
        printf( "Framebuffer Error\n" );
        return false;
    }

    return true;
}

bool nsfw::Assets::makeTexture( const char* name, unsigned w, unsigned h, unsigned depth, const char* pixels ) {
//#pragma message ( __WARN__ "Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value." )
    ASSET_LOG( GL_HANDLE_TYPE::TEXTURE );
    using namespace gl;
    unsigned int tex;
    glGenTextures( 1, &tex );
    handles[AssetKey( GL_HANDLE_TYPE::TEXTURE, name )] = tex;
    glBindTexture( GLenum::GL_TEXTURE_2D, tex );
    if( pixels == nullptr ) {
        char* pix = new char[w * h * depth];
        for ( unsigned int n = 0; n < w*h * depth; ++n ) { pix[n] = 0; }
        glTexImage2D( GLenum::GL_TEXTURE_2D, 0, depth, w, h, 0, GLenum::GL_RGB, GLenum::GL_UNSIGNED_BYTE, pix );
    } 
    else {
        glTexImage2D( GLenum::GL_TEXTURE_2D, 0, depth, w, h, 0, GLenum::GL_RGB, GLenum::GL_UNSIGNED_BYTE, pixels );
    }
    glTexParameteri( GL_TEXTURE_2D, GLenum::GL_TEXTURE_MAG_FILTER, (int)GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GLenum::GL_TEXTURE_MIN_FILTER, (int)GL_LINEAR );
    glBindTexture( GLenum::GL_TEXTURE_2D, 0 );
    return true;
}

bool nsfw::Assets::loadTexture( const char* name, const char* path ) {
//#pragma message ( "This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!")
    using namespace std;
    ifstream file( path );
    if ( !file.good() ) { file.close(); return false; }
    file.close();
    int w = 0, h = 0, format = 0;
    unsigned char* pixels = stbi_load( path, &w, &h, &format, STBI_rgb_alpha );
    makeTexture( name, w, h, (int)gl::GLenum::GL_RGBA, (char*)pixels );
    stbi_image_free( pixels );
    return false;
}

bool nsfw::Assets::loadShader( const char* name, const char* vpath, const char* fpath ) {
#pragma message ( __WARN__ "Load shader from a file.")
    ASSET_LOG( GL_HANDLE_TYPE::SHADER );
    using namespace gl;
    using namespace std;

    string vertexCode, fragmentCode;
    // Did we load the files?
    if ( !loadText( vertexCode, "./Assets/Shaders/VertexShader.glsl" ) ||
         !loadText( fragmentCode, "./Assets/Shaders/FragmentShader.glsl" ) ) {
        cout << "Failed to load shader source files." << endl;
        return false;
    }

    unsigned int program = 0, vertexShader = 0, fragmentShader = 0;

    const char* vertexSource = vertexCode.c_str();
    int sourceSize = vertexCode.size();
    vertexShader = glCreateShader( GLenum::GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &vertexSource, &sourceSize );
    glCompileShader( vertexShader );

    const char* fragmentSource = fragmentCode.c_str();
    sourceSize = fragmentCode.size();
    fragmentShader = glCreateShader( GLenum::GL_VERTEX_SHADER );
    glShaderSource( fragmentShader, 1, &fragmentSource, &sourceSize );
    glCompileShader( fragmentShader );

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
        return false;
    }

    handles[AssetKey( GL_HANDLE_TYPE::SHADER, name )] = program;

    return true;
}

bool nsfw::Assets::loadFBX( const char* name, const char* path ) {
    //name/meshName
    //name/textureName
#pragma message ( __WARN__ "FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n")
    using namespace std;

    if ( !fileExists( path ) ) { return false; }

    FBXFile fbx_file;
    if ( !fbx_file.load( path ) ) {
        cout << "Error: Failed to load FBX file \"" << path << "\"" << endl;
        return false;
    }

    for ( int n = 0; n < fbx_file.getMeshCount(); ++n ) {
        FBXMeshNode* mesh = fbx_file.getMeshByIndex( n );
        string meshName = name + mesh->m_name;
        vector<Vertex> verts( mesh->m_vertices.size() );
        for ( int nn = 0; n < verts.size(); ++nn ) {
            verts[nn].position = mesh->m_vertices[nn].position;
            verts[nn].normal = mesh->m_vertices[nn].normal;
            verts[nn].tangent = mesh->m_vertices[nn].tangent;
            verts[nn].texCoord = mesh->m_vertices[nn].texCoord1;
        }
        makeVAO( meshName.c_str(), verts.data(), verts.size(), mesh->m_indices.data(), mesh->m_indices.size() );
    }

    for ( int n = 0; n < fbx_file.getTextureCount(); ++n ) {
        FBXTexture* texture = fbx_file.getTextureByIndex( n );
        string textureName = name + texture->name;
        makeTexture( textureName.c_str(), texture->width, texture->height, texture->format, (char*)texture->data );
    }

    fbx_file.unload();

    return true;
}

bool nsfw::Assets::loadOBJ( const char* name, const char* path ) {
//#pragma message ( __WARN__ "OBJ file-loading support needed. This function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).")

    using namespace std;
    using namespace tinyobj;
    if( !fileExists( path ) ) { return false; }

    vector<shape_t> shapes;
    vector<material_t> materials;
    string err = LoadObj( shapes, materials, path );

    if( shapes.empty() ) {
        cout << "Error: Failed to load OBJ file \"" << path << "\"" << endl;
        return false;
    }

    for ( int n = 0; n < shapes.size(); ++n )
    {
        using namespace glm;
        string shapeName = name + shapes[n].name;
        vector<Vertex> verts( shapes[n].mesh.positions.size() / 3 );
        for ( int nn = 0; nn < verts.size(); ++nn ) {
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

    if( !fileExists( path ) ) { return false; }

    string line;
    ifstream file( path );
    fileContent.clear();
    while( getline( file, line ) ) {
        fileContent += line;
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
#pragma message ( __WARN__ "Shader deletion" )
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
