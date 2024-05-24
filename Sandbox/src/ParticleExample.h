#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class ParticleExample
{
public:
	//��������,Ӧ����0��800.λ��
	static void Assemble() {
		Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
		Ref<AssetManager> assetManager = GlobalContext::GetInstance()->m_AssetManager;

		Entity emitter = scene->CreateEntity("Emitter");
		//����transform���
		TransformComponent emitterTrans;
		emitterTrans.Translation = glm::vec3{ 800,20,0.3f };
		emitterTrans.Scale = glm::vec3{ 1,1,1 };
		scene->GetComponentWithName<TransformComponent>("Emitter") = emitterTrans;
		//����һ���µ����ӷ���������
		Ref<ParticleEmitter> emitter_p = CreateRef<ParticleEmitter>(100);
		//�趨����
		emitter_p->SetTexture(assetManager->GetTexture("bullet"));
		//�趨��ʼ��
		emitter_p->SetParticleInit([](Particle& p) {
			//��ȡ�������,�����ʼλ��
			p.position.x = p.random_seed * 1600 - 800;
			RPG2D_INFO("random_seed:{0}",p.random_seed);
			p.position.y = 0;
			p.velocity = glm::vec2(0.0,100.0f);
			p.size = glm::vec2(10.0f, 10.0f);
			p.life = 5.0f;
		});
		//���ø��·�ʽ
		emitter_p->SetParticleUpdate([](Particle& p, float dt) {
			//����λ��
			p.position += p.velocity * dt;
		});
		//������ӷ��������
		ParticleEmitterComponent emitterComponent;
		emitterComponent.particleEmitter = emitter_p;
		emitter.AddComponent<ParticleEmitterComponent>(emitterComponent);
	}
	static void OnClick() {
		RPG2D_INFO("emitter click");
	}
};

