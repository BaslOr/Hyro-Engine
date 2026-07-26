#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>

#include "Hyro/Core/Core.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

namespace Hyro {

	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragPath)
		: m_Program(0)
	{
		std::string vertexSource = ReadShaderFromFile(vertexPath);
		std::string fragmentSource = ReadShaderFromFile(fragPath);

		const char* vertexShaderSourceCStr = vertexSource.c_str();
		const char* fragmentShaderSourceCStr = fragmentSource.c_str();
		uint32_t vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSourceCStr);
		uint32_t fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCStr);

		m_Program = glCreateProgram();
		glAttachShader(m_Program, vertexShader);
		glAttachShader(m_Program, fragmentShader);
		glLinkProgram(m_Program);
		CheckLinkingErrors();

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		auto vertexSpirV = ShaderCompiler::CompileToSpirv(vertexPath, ShaderStage::Vertex);
		auto fragmentSpriV = ShaderCompiler::CompileToSpirv(fragPath, ShaderStage::Fragment);
		m_VertexLayout = ShaderReflection::GetVertexLayout(vertexSpirV);

		std::array<std::vector<uint32_t>, 2> spirVs = { vertexSpirV, fragmentSpriV };
		m_ReflectionData = ShaderReflection::FillReflectionData(spirVs);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_Program);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void OpenGLShader::Bind(void* commandBuffer) const
	{
		HYRO_LOG_CORE_WARN("Tried to bind Shader with Command Buffer. This may indicate a bug.");
	}

	void OpenGLShader::SetUnifrom(Uniform uniform)
	{
		glUseProgram(m_Program);
		int location = GetUniformLocation(uniform.Name);

		switch (uniform.Type)
		{
		case DescriptorType::Float:
			glUniform1f(location, *(float*)uniform.Data);
			break;

		case DescriptorType::Float2:
			glUniform2fv(location, 1, (float*)uniform.Data);
			break;

		case DescriptorType::Float3:
			glUniform3fv(location, 1, (float*)uniform.Data);
			break;

		case DescriptorType::Float4:
			glUniform4fv(location, 1, (float*)uniform.Data);
			break;

		case DescriptorType::Int:
			glUniform1i(location, (int)uniform.Data);
			break;

		case DescriptorType::Int2:
			glUniform2iv(location, 1, (int*)uniform.Data);
			break;

		case DescriptorType::Int3:
			glUniform3iv(location, 1, (int*)uniform.Data);
			break;

		case DescriptorType::Int4:
			glUniform4iv(location, 1, (int*)uniform.Data);
			break;

		case DescriptorType::Matrix:
			glUniformMatrix4fv(location, 1, GL_FALSE, (float*)uniform.Data);
			break;

		case DescriptorType::Sampler:
		case DescriptorType::Image:
			glUniform1i(location, (int)uniform.Data);
			break;

		case DescriptorType::UniformBuffer:
		case DescriptorType::StorageBuffer:
			break;
		}
	}

	void OpenGLShader::CheckShaderCompilation(uint32_t shader)
	{
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			HYRO_LOG_CORE_ERROR("Shader compilation failed! \n {}", infoLog);
		}
	}

	void OpenGLShader::CheckLinkingErrors()
	{
		int success;
		char infoLog[512];
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			HYRO_LOG_CORE_ERROR("Shader linking failed! \n {}", infoLog);
		}
	}

	uint32_t OpenGLShader::CreateShader(uint32_t shaderType, const char* shaderSource)
	{
		uint32_t shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);
		CheckShaderCompilation(shader);

		return shader;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_Program, name.c_str());
		m_UniformLocationCache[name] = location;

		if (!CheckLocation(location))
		{
			HYRO_LOG_CORE_ERROR("Failed to get uniform location for: {}", name);
		}
		return location;
	}

	bool OpenGLShader::CheckLocation(int location) const
	{
		if (location == -1) {
			return false;
		}
		return true;
	}

	std::string OpenGLShader::ReadShaderFromFile(const std::string& filePath)
	{
		std::ifstream file(filePath);

		if (!file.is_open()) {
			std::cerr << "Failed to open File: " << filePath << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

}
