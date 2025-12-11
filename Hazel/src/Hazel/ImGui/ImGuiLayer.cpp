#include "hzpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Hazel/Application.h"

namespace Hazel
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	//��ʼ��
	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		
		
		ImGui_ImplOpenGL3_Init("#version 410");

		
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(OnMouseButtonPressedEvent));

		dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(OnMouseButtonReleasedEvent));

		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMovedEvent));

		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnMouseScrolledEvent));

		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressedEvent));
		
		dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(OnKeyTypedEvent));

		dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(OnKeyReleasedEvent));

		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel = e.GetYOffset();
		io.MouseWheelH = e.GetYOffset();
		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		return false;
	}

	//�����¼�
	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());


		float time = (float)glfwGetTime();
		io.DeltaTime = mTime > 0.0f ? (time - mTime) : (1.0f / 60.0f);
		mTime = time;

		ImGui_ImplOpenGL3_NewFrame();
		
		

		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
