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
	//只包含刚体、碰撞体和基本组件
	//地面只是一个碰撞盒，配合背景图片用的
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Entity ground = scene->CreateEntity("Ground");
	//设置transform组件
	TransformComponent groundTrans;
	groundTrans.Translation = glm::vec3{ 0,715,0.3f };
	groundTrans.Scale = glm::vec3{ 1,1,1 };
	scene->GetComponentWithName<TransformComponent>("Ground") = groundTrans;
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Static;
	scene->AddComponentWithName<Rigidbody2DComponent>("Ground", rgb2d);
	//碰撞盒
	BoxCollider2DComponent box2d;
	//设置size和offst。
	//获取物体实际大小
	box2d.Size = glm::vec2(2945, 102);
	//因为box2d y轴向上，opengl y轴向下，offset.y为负值
	box2d.Offset = glm::vec2(0,0);
	scene->AddComponentWithName<BoxCollider2DComponent>("Ground", box2d);
	//将物体加入到物理引擎中。
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(ground);
	return ground;
}


