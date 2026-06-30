#pragma once
#include "Hyro/Renderer/Shader.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <glm/gtc/type_ptr.hpp>

namespace Hyro {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragPath);
		~OpenGLShader();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

	private:
		void CheckShaderCompilation(uint32_t shader);
		void CheckLinkingErrors();

		uint32_t CreateShader(uint32_t shaderType, const char* shaderSource);

		int GetUniformLocation(const std::string& name) const;
		void CheckLocation(int location) const;

		std::string ReadShaderFromFile(const std::string& filePath);

	private:
		uint32_t m_Program;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	};

}
