#include <Hazel.h>
#include "../vendor/imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	virtual void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		{
			HZ_INFO("W");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello Wyy");
		ImGui::End();
	}

	virtual void OnEvent(Hazel::Event& event) override
	{
		HZ_TRACE("{0}", event);
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
