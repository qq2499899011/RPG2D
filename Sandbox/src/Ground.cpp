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
void Ground::Assemble()
{
    std::vector<glm::vec4> dataArray = {
    {32, 112, 736, 8},
    {160, 144, 96, 8},
    {256, 176, 32, 8},
    {288, 208, 64, 8},
    {352, 176, 32, 8},
    {416, 144, 64, 8},
    {608, 208, 64, 8},
    {640, 160, 96, 8},
    {768, 112, 128, 8},
    {896, 112, 160, 8},
    {1056, 112, 128, 8},
    {1184, 112, 256, 8},
    {1376, 80, 512, 8},
    {1408, 208, 96, 8},
    {1504, 160, 64, 8},
    {1600, 144, 224, 8},
    {1728, 208, 192, 8},
    {1856, 112, 224, 8},
    {1920, 176, 64, 8},
    {2016, 176, 64, 8},
    {2048, 80, 160, 8},
    {2112, 160, 32, 8},
    {2176, 144, 96, 8},
    {2240, 112, 64, 8},
    {2336, 144, 64, 8},
    {2336, 208, 32, 8},
    {2368, 176, 96, 8},
    {2464, 112, 64, 8},
    {2496, 80, 64, 8},
    {2496, 208, 32, 8},
    {2528, 160, 32, 8},
    {2592, 112, 64, 8},
    {2624, 144, 160, 8},
    {2720, 208, 96, 8},
    {2848, 176, 64, 8},
    {2944, 144, 64, 8},
    {3008, 112, 128, 8},
    {3008, 208, 240, 8},
    {3040, 160, 96, 8},
    {3136, 144, 32, 8},
    {3168, 176, 32, 8}
    };
    // 循环处理每一行数据
    for (glm::vec4 data : dataArray) {
        Assemble(glm::vec2(data.x,data.y),glm::vec2(data.z,data.w));
    }
}
//组装位置和大小,整体比例放大了3倍，所以pos和size 也放大三倍
Entity Ground::Assemble(glm::vec2 pos,glm::vec2 size)
{
    //将位置和大小都放大三倍
    pos.x *= 3;
    pos.y *= 3;
    size.x *= 3;
    size.y * 3;
	Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
	Ref<PhysicsSystem> physics = GlobalContext::GetInstance()->m_PhysicsSystem;
	Entity ground = scene->CreateEntity("Ground");
	//设置transform组件
	TransformComponent groundTrans;
	groundTrans.Translation = glm::vec3{pos,0.3f };
	groundTrans.Scale = glm::vec3{ 1,1,1 };
    ground.GetComponent<TransformComponent>() = groundTrans;
	//物理组件
	Rigidbody2DComponent rgb2d;
	rgb2d.Type = Rigidbody2DComponent::BodyType::Static;
    ground.AddComponent<Rigidbody2DComponent>(rgb2d);
	//创建一个白色检测盒
	SquareRendererComponent square;
	square.Size = size;
    ground.AddComponent<SquareRendererComponent>(square);
	//碰撞盒
	BoxCollider2DComponent box2d;
	//直接计算物体实际大小
    box2d.isSensor = true;
	physics->CalculateCollisionBox(groundTrans, square.Size, box2d);
    ground.AddComponent<BoxCollider2DComponent>(box2d);
	//将物体加入到物理引擎中。
	GlobalContext::GetInstance()->m_PhysicsSystem->AddEntity(ground);
	//再加入别的碰撞体
	return ground;
}


