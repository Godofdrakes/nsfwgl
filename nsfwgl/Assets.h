#pragma once
#include <unordered_map>
#include <string>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

/*
	The asset management object is a place to keep track of all of the memory
	stored on the video card. From this, we can load data from files, or generate
	new objects as they are needed.

	
	In it are two objects.

	The Asset struct contains all of the information necessary to fetch the openGL handle
	from the Assets object. It is templated for typing purposes.

	Assets is the super container for all openGL handles. A safe place to ensure everything
	is properly liberated and allocated and never lost. It also allows us to ensure
	objects don't load the same thing multiple times.

	usage:

	To load an image into the asset library->
		auto &a = Assets::instance();

		a.loadTexture("JohnDiffuseMap","/path/to/file/john/diffuse.png");

	To create an asset reference->
		Asset<TEXTURE> diffuseMap = "JohnDiffuseMap";	

	To use the asset reference to fetch the handle, any of the following will work:
		a[diffuseMap];
		a.get(diffuseMap);
		a.get<TEXTURE>("JohnDiffuseMap");
		a.get(TEXTURE,"JohnDiffuseMap");
		

		*** Implement the necessary LOAD and MAKE functions, as well as the TERM function.
*/

namespace nsfw {
    // Keep track of the type of handle
    namespace ASSET {
        enum GL_HANDLE_TYPE { eNONE, VAO, IBO, VBO, SIZE, FBO, RBO, TEXTURE, SHADER, eSIZE };

    }

    extern const char* TYPE_NAMES[];
    // Use a handle type and name to use as an index for each asset
    typedef std::pair<ASSET::GL_HANDLE_TYPE, std::string> AssetKey;

    // Asset reference object, used to keep a type and name associated with the
    // manner in which the Assets arranges its keys. This is purely sugar.
    template<ASSET::GL_HANDLE_TYPE T>
    struct Asset {
        const ASSET::GL_HANDLE_TYPE t;
        std::string name;

        Asset() : t( T ), name( "" ) {}

        Asset( const char* n ) : name( n ), t( T ) {} // construct w/string if desired
        Asset& operator=( const char* s ) {
            name = s;
            return *this;
        } // conviently assign strings directly to reference		
        operator AssetKey() const {
            return AssetKey( t, name );
        } // for use with Assets::Operator[]
        gl::GLuint operator*() const {
            return Assets::instance()[*this];
        } // Overload value-of operator, to dereference
        const void* operator&() const {
            return Assets::instance().getUNIFORM( *this );
        }

        operator const void*() const {
            return Assets::instance().getUNIFORM( *this );
        }
    };

    /*
		Asset management singleton. It should be the only place that the glGen**** functions
		are called, and the only place where glDelete/Destroy/etc. are called. It should
		serve no other purpose than to provide a concise location where all openGL handles are
		kept track of and to provide a layer of indirection between objects in the game and
		the actual asset on the video card.
	*/
    class Assets {
    private:
        // Hashing functor object for accepting pair<enum,string> as an index.
        struct Hash {
            size_t operator()( AssetKey k ) const {
                return std::hash<std::string>()( k.second ) + ( unsigned )k.first;
            }
        };

        // Store all of our keys in one place!
        std::unordered_map<AssetKey, gl::GLuint, Hash> handles;

        Assets() {}

        gl::GLuint getVERIFIED( const AssetKey& key ) const;

        bool setINTERNAL( ASSET::GL_HANDLE_TYPE t, const char* name, gl::GLuint handle );

    public:
        // Singleton accessor
        static Assets& instance() {
            static Assets a;
            return a;
        }

        //normal get handle function
        gl::GLuint get( ASSET::GL_HANDLE_TYPE t, const char* name ) const {
            return getVERIFIED( AssetKey( t, name ) );
        }

        //templated Get,for sexiness
        template<ASSET::GL_HANDLE_TYPE t>
        gl::GLuint get( const char* name ) const {
            return getVERIFIED( AssetKey( t, name ) );
        }

        // Get via the Asset reference, sexier
        gl::GLuint get( const AssetKey& key ) const {
            return getVERIFIED( key );
        }

        //Conveniently fetch handle using an Asset object, for even more sexy
        gl::GLuint operator[]( const AssetKey& key ) const {
            return getVERIFIED( key );
        }

        const void* getUNIFORM( const AssetKey& key ) {
            return &handles.find( key )._Ptr->_Myval.second;
        }


        /////////////////////
        // Fill these out! //
        /////////////////////

        // Should also allocate for IBO and VBO
        bool makeVAO( const char* name, const struct Vertex* verts, unsigned vsize, const unsigned* tris, unsigned tsize );

        // should call makeTexture nTextures number of times
        bool makeFBO( const char* name, unsigned w, unsigned h, unsigned nTextures, const char* names[], const gl::GLenum depths[] );

        // should allocate space for a texture, but not necessarily set its data
        bool makeTexture( const char* name, unsigned w, unsigned h, gl::GLenum depth, const char* pixels = nullptr );

        // should load a texture from a file, use makeTexture to alloc, and then copy filedata in
        bool loadTexture( const char* name, const char* path );

        // should load a shader from file
        bool loadShader( const char* name, const char* vpath, const char* fpath );

        unsigned int compileShader( gl::GLenum type, const char* path );

        // should load from an FBX, adding assets to the library as they are discovered
        bool loadFBX( const char* name, const char* path );

        // Should load an OBJ from file, adding appropriate assets to the library
        bool loadOBJ( const char* name, const char* path );

        // Checks a file exists
        bool fileExists( const char* path );

        // Loads all text from selected file at path into fileContent
        bool loadText( std::string& fileContent, const char* path );

        //load some default assets
        void init();
        // clear out all of the opengl handles!
        void term();
    };
}
