#pragma once

namespace Hazel
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : mTime(time)
		{

		}

		//提供将对象转化为float的方法
		operator float() const
		{
			return mTime;
		}

	private:
		float mTime;

	public:
		float GetSeconds() const
		{
			return mTime;
		}

		float GetMilliseconds() const
		{
			return mTime * 1000.0f;
		}
	};
}