#include "hzpch.h"
#include "Shader.h"

#include "glad/glad.h"

namespace Hazel
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) : mRendererID(0)
	{
		unsigned int vertexShaderID = glCreateShader(1);
		const char* vertex = vertexSrc.c_str();
		glShaderSource(vertexShaderID, 1, &vertex, nullptr);
		glCompileShader(vertexShaderID);

		unsigned int fragmentShaderID = glCreateShader(1);
		const char* fragment = fragmentSrc.c_str();
		glShaderSource(fragmentShaderID, 1, &fragment, nullptr);
		glCompileShader(fragmentShaderID);

		mRendererID = glCreateProgram();
		unsigned int program = mRendererID;
		glAttachShader(program, vertexShaderID);
		glAttachShader(program, fragmentShaderID);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Shader::~Shader()
	{
		glDeleteProgram(mRendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}


