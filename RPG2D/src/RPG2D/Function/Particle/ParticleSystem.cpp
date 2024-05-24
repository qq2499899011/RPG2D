#include "RPG2Dpch.h"
#include "ParticleSystem.h"
namespace RPG2D {
	void ParticleSystem::Update(Timestep ts)
	{
		//遍历所有带有粒子发射器和变换组件的内容
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		auto group1 = m_Registry->view<TransformComponent, ParticleEmitterComponent>();
		for (auto entity : group1)
		{
			auto [trans, emitter] = group1.get<TransformComponent, ParticleEmitterComponent>(entity);
			//获取粒子发射器，更新粒子最新动态
			emitter.particleEmitter->Update(ts);
			//渲染粒子,这个部分或许可以在渲染系统处理。
		}
	}
}
