#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/KeyEvent.h"
#include "Hazel/Event/MounseEvent.h"

#include <glad/glad.h>



namespace Hazel
{
	static bool sGLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{

	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		mData.Title = props.title;
		mData.Width = props.width;
		mData.Height = props.height;

		HZ_CORE_INFO("Creating window {0} {{{1}, {2}}}", props.title, props.width, props.height);

		if (!sGLFWInitialized)
		{
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			sGLFWInitialized = true;
		}

		mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(mWindow); //创建上下文环境
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //初始化Glad
		HZ_CORE_ASSERT(status, "Failed to initailize Glad!");

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);


		//设置回调函数
		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;
				//std::cout << width << std::endl;
				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					{
					case GLFW_PRESS:
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}

					{
					case GLFW_RELEASE:
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}

					{
					case GLFW_REPEAT:
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		//打字回调
		glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent e(keycode);
				data.EventCallback(e);
			});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					{
					case GLFW_PRESS:
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					
					{
					case GLFW_RELEASE:
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		//鼠标移动回调
		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(mWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		//处理事件
		glfwPollEvents();
		//交换前后缓冲区
		glfwSwapBuffers(mWindow);
	}

	//设置垂直同步
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		mData.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return mData.VSync;
	}
}
