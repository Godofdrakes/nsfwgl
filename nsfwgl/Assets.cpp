#include "nsfw.h"

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


bool nsfw::Assets::makeVAO( const char* name, const struct Vertex* verts, unsigned vsize, const unsigned* tris, unsigned tsize ) {
    ASSET_LOG(GL_HANDLE_TYPE::VBO);
    ASSET_LOG(GL_HANDLE_TYPE::IBO);
    ASSET_LOG(GL_HANDLE_TYPE::VAO);
    ASSET_LOG(GL_HANDLE_TYPE::SIZE);
#pragma message ( __ERR__ "Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!")
    return false;
}

bool nsfw::Assets::makeFBO( const char* name, unsigned w, unsigned h, unsigned nTextures, const char* names[], const unsigned depths[] ) {
    ASSET_LOG(GL_HANDLE_TYPE::FBO);
#pragma message ( __ERR__ "Create an FBO! Array parameters are for the render targets, which this function should also generate!\nuse makeTexture.\nNOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.")
    return false;
}

bool nsfw::Assets::makeTexture( const char* name, unsigned w, unsigned h, unsigned depth, const char* pixels ) {
    ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
#pragma message ( __ERR__ "Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.")
    return false;
}

bool nsfw::Assets::loadTexture( const char* name, const char* path ) {
#pragma message ( __ERR__ "This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!")
    return false;
}

bool nsfw::Assets::loadShader( const char* name, const char* vpath, const char* fpath ) {
    ASSET_LOG(GL_HANDLE_TYPE::SHADER);
#pragma message ( __ERR__ "Load shader from a file.")
    return false;
}

bool nsfw::Assets::loadFBX( const char* name, const char* path ) {
    //name/meshName
    //name/textureName
#pragma message ( __ERR__ "FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n")
    return false;
}

bool nsfw::Assets::loadOBJ( const char* name, const char* path ) {
#pragma message ( __ERR__ "OBJ file-loading support needed.\nThis function should call makeVAO and loadTexture (if necessary), MAKE SURE TO TAKE THE OBJ DATA AND PROPERLY LINE IT UP WITH YOUR VERTEX ATTRIBUTES (or interleave the data into your vertex struct).\n")
    return false;
}

void nsfw::Assets::init() {
#pragma message ( __ERR__ "Load up some default assets here if you want.")

    setINTERNAL( FBO, "Screen", 0 );

    makeVAO( "Cube", CubeVerts, 24, CubeTris, 36 );
    makeVAO( "Quad", QuadVerts, 4, QuadTris, 6 );
    /*
	char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGB8, w);
	*/

}

void nsfw::Assets::term() {
#pragma message ( __ERR__ "" )
    for each ( std::pair<AssetKey,unsigned> k in handles ) {
        switch ( k.first.first ) {
            case VBO :
#pragma message ( __ERR__ "VBO deletion" )
                break;
            case IBO :
#pragma message ( __ERR__ "IBO deletion" )
                break;
            case VAO :
#pragma message ( __ERR__ "VAO deletion" )
                break;
            case SHADER :
#pragma message ( __ERR__ "Shader deletion" )
                break;
            case TEXTURE :
#pragma message ( __ERR__ "Texture deletion" )
                break;
            case RBO :
#pragma message ( __ERR__ "RBO deletion" )
                break;
            case FBO :
#pragma message ( __ERR__ "FBO deletion" )
                break;
        }
    }
}
