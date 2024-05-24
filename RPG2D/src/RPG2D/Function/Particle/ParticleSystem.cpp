#include "RPG2Dpch.h"
#include "ParticleSystem.h"
namespace RPG2D {
	void ParticleSystem::Update(Timestep ts)
	{
		//�������д������ӷ������ͱ任���������
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		auto group1 = m_Registry->view<TransformComponent, ParticleEmitterComponent>();
		for (auto entity : group1)
		{
			auto [trans, emitter] = group1.get<TransformComponent, ParticleEmitterComponent>(entity);
			//��ȡ���ӷ������������������¶�̬
			emitter.particleEmitter->Update(ts);
			//��Ⱦ����,������ֻ����������Ⱦϵͳ����
		}
	}
}
