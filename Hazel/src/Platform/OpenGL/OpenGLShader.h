#pragma once

#include "../../../vendor/glm/glm.hpp"
#include "Hazel/Renderer/Shader.h"

namespace Hazel
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t mRendererID;

	public:
		void UploadUniformInt(const std::string& name, const int value) const;

		void UploadUniformFloat(const std::string& name, const float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec2) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec3) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec4) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	};
}
