#pragma once

#include "Core/Mesh/Primitives/mesh.h"
#include "Core/Texture/texture.h"

/**
 * @brief Rectangular texture representation
 * 
 */
class Image : public Mesh {
public:
    Image(Texture &texture);
private:
    Texture * texture = nullptr;
}