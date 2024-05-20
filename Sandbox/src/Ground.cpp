#include "Ground.h"
void Ground::OnCreate()
{
}

void Ground::OnDestroy()
{
}

void Ground::OnUpdate(Timestep ts)
{
}


Entity Ground::Assemble()
{
	//ֻ�������塢��ײ��ͻ������
	//����ֻ��һ����ײ�У���ϱ���ͼƬ�õ�
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Entity ground = scene->CreateEntity("Ground");
	//����transform���
	TransformComponent groundTrans;
	groundTrans.Translation = glm::vec3{ 0,715,0.3f };
	groundTrans.Scale = glm::vec3{ 1,1,1 };
	scene->GetComponentWithName<TransformComponent>("Ground") = groundTrans;
	//�������
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Static;
	scene->AddComponentWithName<Rigidbody2DComponent>("Ground", rgb2d);
	//��ײ��
	BoxCollider2DComponent box2d;
	//����size��offst��
	//��ȡ����ʵ�ʴ�С
	box2d.Size = glm::vec2(2945, 102);
	//��Ϊbox2d y�����ϣ�opengl y�����£�offset.yΪ��ֵ
	box2d.Offset = glm::vec2(0,0);
	scene->AddComponentWithName<BoxCollider2DComponent>("Ground", box2d);
	//��������뵽���������С�
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(ground);
	return ground;
}


