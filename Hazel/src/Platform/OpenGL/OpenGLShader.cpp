#include "hzpch.h"
#include "OpenGLShader.h"

#include "glad/glad.h"
#include<gtc/type_ptr.hpp>

namespace Hazel
{
	//创建着色器程序
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) : mRendererID(0)
	{
		//创建顶点着色器
		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char* vertex = vertexSrc.c_str();
		glShaderSource(vertexShaderID, 1, &vertex, nullptr);
		glCompileShader(vertexShaderID);

		// 检查顶点着色器编译错误
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//创建片段着色器
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragment = fragmentSrc.c_str();
		glShaderSource(fragmentShaderID, 1, &fragment, nullptr);
		glCompileShader(fragmentShaderID);

		// 检查片段着色器编译错误
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		mRendererID = glCreateProgram();
		unsigned int program = mRendererID;
		glAttachShader(program, vertexShaderID);
		glAttachShader(program, fragmentShaderID);
		glLinkProgram(program);

		// 检查链接错误
		glGetProgramiv(mRendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mRendererID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glValidateProgram(program);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	
	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec2) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform2f(location, vec2.x, vec2.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec3) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform3f(location, vec3.x, vec3.y, vec3.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec4) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}