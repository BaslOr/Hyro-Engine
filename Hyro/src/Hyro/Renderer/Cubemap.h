#pragma once
#include "Hyro/Core/Memory.h"
#include "Hyro/Utils/Bitmap.h"

#include <glm/fwd.hpp>

namespace Hyro {

	class Cubemap {
	public:
		static Ref<Cubemap> Create(const std::string& filepath);

		virtual void Bind() const = 0;		
		virtual void Unbind() const = 0;

		static glm::vec3 FaceCoordsToXYZ(int i, int j, int faceID, int faceSize);
		static Bitmap ConvertEquirectangularMapToVerticalCross(const Bitmap& bitmap);
		static Bitmap ConvertVerticalCrossToCubeMapFaces(const Bitmap& bitmap);

	};

}