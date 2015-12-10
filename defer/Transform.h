#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace nsfw {

    class Transform {
    protected:
        glm::vec3 m_position;
        glm::vec3 m_rotation; // Rotation in degrees
        glm::vec3 m_scale;
    public:
        // Constructors
        Transform( glm::vec3 position = glm::vec3( 0 ),
                   glm::vec3 rotation = glm::vec3( 0 ),
                   glm::vec3 scale = glm::vec3( 1 ) ) :
            m_position( position ),
            m_rotation( rotation ),
            m_scale( scale ) {}

        virtual ~Transform() {}

        // Getters
        virtual glm::vec3 Get_Position() const {
            return m_position;
        }

        virtual glm::vec3 Get_Rotation() const {
            return m_rotation;
        }

        virtual glm::vec3 Get_Scale() const {
            return m_scale;
        }

        // Setters
        virtual void Set_Position( const glm::vec3 position ) {
            m_position = position;
        }

        virtual void Set_Rotation( const glm::vec3 rotation ) {
            m_rotation = rotation;
        }

        virtual void Set_Scale( const glm::vec3 scale ) {
            m_scale = scale;
        }

        // Properties
        __declspec(property(get = Get_Position, put = Set_Position)) glm::vec3 Position;

        __declspec(property(get = Get_Rotation, put = Set_Rotation)) glm::vec3 Rotation;

        __declspec(property(get = Get_Scale, put = Set_Scale)) glm::vec3 Scale;

        // Matrix Getters
        virtual glm::mat4 GetRoationMatrix() const {
            return glm::rotate( Rotation.z, glm::vec3( 0, 0, 1 ) ) *
                    glm::rotate( Rotation.y, glm::vec3( 0, 1, 0 ) ) *
                    glm::rotate( Rotation.x, glm::vec3( 1, 0, 0 ) );
        }

        glm::mat4 GetTranslationMatrix() const {
            return glm::translate( Position );
        }

        glm::mat4 GetScaleMatrix() const {
            return glm::scale( Scale );
        }

        virtual glm::mat4 GetWorldTransform() const {
            return glm::translate( Position ) *
                    GetRoationMatrix() *
                    glm::scale( Scale );
        }
    };

}

