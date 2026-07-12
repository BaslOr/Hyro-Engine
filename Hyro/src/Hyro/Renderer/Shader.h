#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Hyro/Core/Memory.h"

#include "Hyro/Renderer/Vertex.h"


namespace Hyro {

	struct PushConstants {
		glm::mat4 Model;
	};


	class Shader {
	public:
		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual VertexLayout GetVertexLayout() const = 0;

		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
	};

}
