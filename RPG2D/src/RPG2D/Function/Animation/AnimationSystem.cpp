#include "RPG2Dpch.h"
#include "AnimationSystem.h"
namespace RPG2D {
	void AnimationSystem::Init()
	{
		//目前什么都不做
	}
	void AnimationSystem::Update(Timestep ts)
	{
		//遍历执行update函数
		//获取animatior和spriteRenderer组件。更新组件内容
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		{
			auto group = m_Registry->group<AnimatiorControllerComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [animatior, sprite] = group.get<AnimatiorControllerComponent, SpriteRendererComponent>(entity);
				//更新动画，获取当前animator对应的texture
				animatior.animatiorController->Update(ts);
				//更新texture2D。
				sprite.Texture = animatior.animatiorController->GetCurFrame();
			}
		}
	}
}
