#pragma once
#include "Hyro/Core/Memory.h"
#include <string>
#include <glm/glm.hpp>

namespace Hyro {

	class Shader {
	public:
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetUniformInt(const std::string& name, int vlaue) const = 0;
		virtual void SetUniformFloat(const std::string& name, float value) const = 0;
		virtual void SetUnifromBool(const std::string& name, bool value) const = 0;
		virtual void setUniformVec3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetUniformVec4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void setUniformMat4(const std::string& name, const glm::mat4& value) const = 0;

		static Ref<Shader> Create(const std::string& vertexPath, const std::string& fragmentPath);
	};

}
