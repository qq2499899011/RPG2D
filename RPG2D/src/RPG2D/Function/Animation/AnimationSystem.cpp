#include "RPG2Dpch.h"
#include "AnimationSystem.h"
namespace RPG2D {
	void AnimationSystem::Init()
	{
		//Ŀǰʲô������
	}
	void AnimationSystem::Update(Timestep ts)
	{
		//����ִ��update����
		//��ȡanimatior��spriteRenderer����������������
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		{
			auto group = m_Registry->group<AnimatiorControllerComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [animatior, sprite] = group.get<AnimatiorControllerComponent, SpriteRendererComponent>(entity);
				//���¶�������ȡ��ǰanimator��Ӧ��texture
				animatior.animatiorController->Update(ts);
				//����texture2D��
				sprite.Texture = animatior.animatiorController->GetCurFrame();
			}
		}
	}
}
