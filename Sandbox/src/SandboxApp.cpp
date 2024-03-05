#include <RPG2D.h>
class ExampleLayer : public RPG2D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate(RPG2D::Timestep st) override
	{
		//RPG2D_INFO("ExampleLayer::Update");
	}

	void OnEvent(RPG2D::Event& event) override
	{
		//RPG2D_TRACE("{0}", event);//��仰������
	}

};

class Sandbox :public RPG2D::Application {
	public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new RPG2D::ImGuiLayer());
	}
	~Sandbox() {

	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}