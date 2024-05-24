#pragma once
namespace RPG2D {
	struct Particle{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec2 size;
		float rotation;
		glm::vec4 color;//默认白色
		float random_seed;//随机种子，用于计算位置速度
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
			//绑定默认函数
			m_ParticleInit = RPG2D_BIND_EVENT_FN(ParticleEmitter::DefaultParticleInit);
			m_ParticleUpdate = RPG2D_BIND_EVENT_FN(ParticleEmitter::DefaultParticleUpdate);
		}
		void Update(float dt);
		void Emit();
		//Getter Setter函数
			// Getter 和 Setter 函数
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

		//粒子初始化与更新
		void SetParticleInit(const std::function<void(Particle&)>& initFunc) { m_ParticleInit = initFunc; }
		void SetParticleUpdate(const std::function<void(Particle&, float)>& updateFunc) { m_ParticleUpdate = updateFunc; }

	private:
		//储存粒子
		std::vector<Particle> m_Particles;
		//最大粒子数量
		uint64_t m_MaxParticles = 30;
		//最近生成的粒子下标
		uint64_t m_LastUsedParticle = 0;
		//粒子发射器的默认生命值
		float m_ParticleLife = 2.0f;
		//粒子纹理
		Ref<Texture2D> texture;
		//是否是
		bool m_IsTemp = false;
		//发射器持续时间,默认10秒
		float m_EmitterLife = 10.0f;
		//粒子发射间隔时间
		float m_IntTime = 0.5f;
		float m_AccTime = 0.0f;//累计时间
		//左右随机数边界
		float m_RandomLeft = 0.0f;
		float m_RandomRight = 1.0f;
		//支持自定义粒子的初始化和更新方式
		std::function<void(Particle&)> m_ParticleInit;
		std::function<void(Particle&,float)> m_ParticleUpdate;
		void DefaultParticleInit(Particle&);
		void DefaultParticleUpdate(Particle&,float);
		//找到最新的未被使用的粒子
		uint64_t FirstUnusedParticle();
		//重新生成粒子
		void RespawnParticle(Particle& particle);
		float GenerateRandomFloat();
		void Init();//初始化工作
	};
}
