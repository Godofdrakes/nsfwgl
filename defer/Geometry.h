#pragma once

#include "Transform.h"
#include "../nsfwgl/nsfw.h"

namespace nsfw {

    class Geometry : public Transform {
    public:
        nsfw::Asset<ASSET::VAO> mesh;
        nsfw::Asset<ASSET::SIZE> tris;

        nsfw::Asset<ASSET::TEXTURE> diffuse;
        nsfw::Asset<ASSET::TEXTURE> normal;
        nsfw::Asset<ASSET::TEXTURE> specular;

        float specPower;

        Geometry() : Geometry( "Quad", "", "", "", "", 0.0f ) {}

        Geometry( const nsfw::Asset<ASSET::VAO>& mesh, const nsfw::Asset<ASSET::SIZE>& tris, const nsfw::Asset<ASSET::TEXTURE>& diffuse, const nsfw::Asset<ASSET::TEXTURE>& normal, const nsfw::Asset<ASSET::TEXTURE>& specular, float specPower )
            : mesh( mesh ),
              tris( tris ),
              diffuse( diffuse ),
              normal( normal ),
              specular( specular ),
              specPower( specPower ) {}

        virtual ~Geometry() {}

        virtual void Update() {}
    };

}
