#include <RPG2D.h>
class ExampleLayer : public RPG2D::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}
	void OnUpdate(RPG2D::Timestep ts) override
	{
		//RPG2D_INFO("ExampleLayer::Update");
	}
	void OnEvent(RPG2D::Event& event) override
	{
		//RPG2D_TRACE("{0}", event);//这句话有问题
	}
};

//继承init，设置相应场景与初始化
class Sandbox :public RPG2D::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//运行init函数
		Init();
	}
	~Sandbox() {

	}
	//定义场景。
	void Init() {


	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}