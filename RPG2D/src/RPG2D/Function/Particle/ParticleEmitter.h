#pragma once
namespace RPG2D {
	struct Particle{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec2 size;
		float rotation;
		glm::vec4 color;//Ĭ�ϰ�ɫ
		float random_seed;//������ӣ����ڼ���λ���ٶ�
		float life;
		Particle()
			: position(0.0f), velocity(0.0f), color(1.0f), life(0.0f),size(1.0f), rotation(0.0f) {}
	};
	class ParticleEmitter
	{
	public:
		ParticleEmitter(uint64_t m_MaxParticles)
			: m_MaxParticles(m_MaxParticles) {
			m_Particles.resize(m_MaxParticles);
			//��Ĭ�Ϻ���
			m_ParticleInit = RPG2D_BIND_EVENT_FN(ParticleEmitter::DefaultParticleInit);
			m_ParticleUpdate = RPG2D_BIND_EVENT_FN(ParticleEmitter::DefaultParticleUpdate);
		}
		void Update(float dt);
		void Emit();
		//Getter Setter����
			// Getter �� Setter ����
		std::vector<Particle> GetParticles() const { return m_Particles; }

		float GetParticleLife() const { return m_ParticleLife; }
		void SetParticleLife(float life) { m_ParticleLife = life; }

		Ref<Texture2D> GetTexture() const { return texture; }
		void SetTexture(Ref<Texture2D> tex) { texture = tex; }

		float GetEmitterLife() const { return m_EmitterLife; }
		void SetEmitterLife(float time) { m_EmitterLife = time; }

		float GetIntTime() const { return m_IntTime; }
		void SetIntTime(float intTime) { m_IntTime = intTime; }

		float GetAccTime() const { return m_AccTime; }
		void SetAccTime(float accTime) { m_AccTime = accTime; }

		float GetRandomLeft() const { return m_RandomLeft; }
		void SetRandomLeft(float randomLeft) { m_RandomLeft = randomLeft; }

		float GetRandomRight() const { return m_RandomRight; }
		void SetRandomRight(float randomRight) { m_RandomRight = randomRight; }

		//���ӳ�ʼ�������
		void SetParticleInit(const std::function<void(Particle&)>& initFunc) { m_ParticleInit = initFunc; }
		void SetParticleUpdate(const std::function<void(Particle&, float)>& updateFunc) { m_ParticleUpdate = updateFunc; }

	private:
		//��������
		std::vector<Particle> m_Particles;
		//�����������
		uint64_t m_MaxParticles = 30;
		//������ɵ������±�
		uint64_t m_LastUsedParticle = 0;
		//���ӷ�������Ĭ������ֵ
		float m_ParticleLife = 2.0f;
		//��������
		Ref<Texture2D> texture;
		//�Ƿ���
		bool m_IsTemp = false;
		//����������ʱ��,Ĭ��10��
		float m_EmitterLife = 10.0f;
		//���ӷ�����ʱ��
		float m_IntTime = 0.5f;
		float m_AccTime = 0.0f;//�ۼ�ʱ��
		//����������߽�
		float m_RandomLeft = 0.0f;
		float m_RandomRight = 1.0f;
		//֧���Զ������ӵĳ�ʼ���͸��·�ʽ
		std::function<void(Particle&)> m_ParticleInit;
		std::function<void(Particle&,float)> m_ParticleUpdate;
		void DefaultParticleInit(Particle&);
		void DefaultParticleUpdate(Particle&,float);
		//�ҵ����µ�δ��ʹ�õ�����
		uint64_t FirstUnusedParticle();
		//������������
		void RespawnParticle(Particle& particle);
		float GenerateRandomFloat();
		void Init();//��ʼ������
	};
}
