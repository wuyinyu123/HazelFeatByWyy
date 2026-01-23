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
		//HZ_CORE_INFO("OpenGL Info: ");
		//HZ_CORE_INFO("Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		//HZ_CORE_INFO("Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		//HZ_CORE_INFO("Version: {0}", (const char*)glGetString(GL_VERSION));

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
