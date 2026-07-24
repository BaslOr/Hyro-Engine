#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Vertex.h"



namespace Hyro {

	class ShaderReflectionData;

	//Temporary until Push Constants are fully dynamic
	struct PushConstants {
		glm::mat4 Model;
	};


	enum class DescriptorType {
		UNIFORM_BUFFER, Sampler
	};

	enum class ShaderStage {
		Vertex,
		Fragment,
		Compute
	};



	class Shader {
	public:
		virtual void Bind() const = 0;
		virtual void Bind(void* commandBuffer) const = 0;

		virtual VertexLayout GetVertexLayout() const = 0;
		virtual ShaderReflectionData GetReflectionData() const = 0;

		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
	};

}
