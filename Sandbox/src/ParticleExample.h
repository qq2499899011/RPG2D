#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class ParticleExample
{
public:
	//生成粒子,应该是0，800.位置
	static void Assemble() {
		Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
		Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;

		Entity emitter = scene->CreateEntity("Emitter");
		//设置transform组件
		TransformComponent emitterTrans;
		emitterTrans.Translation = glm::vec3{ 800,20,0.3f };
		emitterTrans.Scale = glm::vec3{ 1,1,1 };
		scene->GetComponentWithName<TransformComponent>("Emitter") = emitterTrans;
		//创建一个新的粒子发射器对象
		Ref<ParticleEmitter> emitter_p = CreateRef<ParticleEmitter>(100);
		//设定纹理
		emitter_p->SetTexture(assetManager->GetTexture("bullet"));
		//设定初始化
		emitter_p->SetParticleInit([](Particle& p) {
			//获取随机种子,计算初始位置
			p.position.x = p.random_seed * 1600 - 800;
			RPG2D_INFO("random_seed:{0}",p.random_seed);
			p.position.y = 0;
			p.velocity = glm::vec2(0.0,100.0f);
			p.size = glm::vec2(10.0f, 10.0f);
			p.life = 5.0f;
		});
		//设置更新方式
		emitter_p->SetParticleUpdate([](Particle& p, float dt) {
			//更新位置
			p.position += p.velocity * dt;
		});
		//添加粒子发射器组件
		ParticleEmitterComponent emitterComponent;
		emitterComponent.particleEmitter = emitter_p;
		emitter.AddComponent<ParticleEmitterComponent>(emitterComponent);
	}
	static void OnClick() {
		RPG2D_INFO("emitter click");
	}
};

