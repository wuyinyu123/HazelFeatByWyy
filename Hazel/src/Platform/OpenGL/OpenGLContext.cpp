#include "hzpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : mWindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindowHandle); //创建上下文环境
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //初始化Glad
		HZ_CORE_ASSERT(status, "Failed to initailize Glad!");
	}

	void OpenGLContext::SwapBuffers()
	{
		//交换前后缓冲区
		glfwSwapBuffers(mWindowHandle);
	}
}
