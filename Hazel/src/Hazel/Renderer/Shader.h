#pragma once
#include "../../vendor/glm/glm.hpp"

namespace Hazel
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

	public:
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t mRendererID;

	public:
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
	};
}



