#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Hyro/Core/Memory.h"
#include "Hyro/Renderer/Vertex.h"



namespace Hyro {

	class ShaderReflectionData;


	enum class DescriptorType
	{
		Float,
		Float2,
		Float3,
		Float4,

		Int,
		Int2,
		Int3,
		Int4,

		Matrix,

		Sampler,
		Image,

		UniformBuffer,
		StorageBuffer
	};

	constexpr uint32_t SizeOfDescriptorType(DescriptorType type) {
		switch (type)
		{
		case Hyro::DescriptorType::Float:
			return 4;
		case Hyro::DescriptorType::Float2:
			return 4*2;
		case Hyro::DescriptorType::Float3:
			return 4*3;
		case Hyro::DescriptorType::Float4:
			return 4*4;
		case Hyro::DescriptorType::Int:
			return 4;
		case Hyro::DescriptorType::Int2:
			return 4*2;
		case Hyro::DescriptorType::Int3:
			return 4*3;
		case Hyro::DescriptorType::Int4:
			return 4*4;
		case Hyro::DescriptorType::Matrix:
			return 4*4*4;
		case Hyro::DescriptorType::Sampler:
			return 0;
		case Hyro::DescriptorType::Image:
			return 0;
		case Hyro::DescriptorType::UniformBuffer:
			return 0;
		case Hyro::DescriptorType::StorageBuffer:
			return 0;
		}
	}

	struct Uniform {
		Uniform(const std::string& name, DescriptorType type, void* data)
			: Name(name), Type(type), Data(data) { }

		std::string Name;
		DescriptorType Type;
		void* Data;
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
