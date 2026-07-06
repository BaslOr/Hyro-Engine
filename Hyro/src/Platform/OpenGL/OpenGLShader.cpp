#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>

#include "Hyro/Core/Core.h"


#include <fstream>
#include <sstream>

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

	void OpenGLShader::SetUniformInt(const std::string& name, int value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::SetUnifromBool(const std::string& name, bool value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform1i(location, (int)value);
	}

	void OpenGLShader::setUniformVec3(const std::string& name, const glm::vec3& value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformVec4(const std::string& name, const glm::vec4& value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
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
