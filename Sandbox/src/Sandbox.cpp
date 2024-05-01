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
		//RPG2D_TRACE("{0}", event);//��仰������
	}
};

//�̳�init��������Ӧ�������ʼ��
class Sandbox :public RPG2D::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//����init����
		Init();
	}
	~Sandbox() {

	}
	//���峡����
	void Init() {


	}
};
RPG2D::Application* RPG2D::CreateApplication() {
	return new Sandbox();
}