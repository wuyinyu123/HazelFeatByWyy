#pragma once
#include "../../vendor/glm/glm.hpp"

namespace Hazel
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom , float top);

		const glm::vec3& GetPosition() const
		{
			return mPosition;
		}

		void SetPosition(const glm::vec3& position)
		{
			mPosition = position;
			RecalulateViewMatrix();
		}

		const float GetRotation() const
		{
			return mRotation;
		}

		void SetRotation(const float rotation)
		{
			mRotation = rotation;
			RecalulateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const
		{
			return mProjectionMatrix;
		}

		const glm::mat4& GetViewProjectionMatrix() const
		{
			return mViewProjectionMatrix;
		}

		const glm::mat4& GetViewMatrix() const
		{
			return mViewMatrix;
		}

		

	private:
		void RecalulateViewMatrix();

	private:
		glm::vec3 mPosition = { 0.0f, 0.0f, 0.0f };
		float mRotation = 0.0f;

		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewProjectionMatrix;
	};
}