#pragma once
#include <vector>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Vertex.h"
#include "Hyro/Renderer/Texture.h"


namespace Hyro {

    struct Mesh {
        std::vector<Vertex> Vertices;
        std::vector<uint32_t> Indices;
        Ref<Texture> Texture;
    };

    struct Sprite {
		Ref<Texture> Texture;
    };

}
