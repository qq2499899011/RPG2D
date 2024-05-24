#include "RPG2Dpch.h"
#include "ParticleEmitter.h"
namespace RPG2D {
    //更新粒子状态
    void ParticleEmitter::Update(float dt) {
        //如果已经小于0，直接返回
        if (m_IsTemp) {
            m_EmitterLife -= dt;
            if (m_EmitterLife <= 0.0f)return;
        }
        //更新时间，确定是否发射粒子
        m_AccTime += dt;
        if (m_AccTime >= m_IntTime) {
            m_AccTime -= m_IntTime;
            Emit();
        }
        //遍历所有粒子
        for (uint64_t i = 0; i < m_MaxParticles; ++i) {
            Particle& p = m_Particles[i];
            p.life -= dt; //更新生命周期
            if (p.life > 0.0f) { //如果粒子还活着,调用更新函数
                m_ParticleUpdate(p, dt);
            }
        }
    }
    //发射一个粒子
    void ParticleEmitter::Emit() {
        uint64_t unusedParticle = FirstUnusedParticle();
        RespawnParticle(m_Particles[unusedParticle]);
    }
    void ParticleEmitter::DefaultParticleInit(Particle& particle)
    {
        //默认初始化
        float random = particle.random_seed;
        //初始化随机位置
        particle.position = glm::vec2(0.0f, 0.0f) + random;
        particle.color = glm::vec4(1.0f);
        particle.life = m_ParticleLife;
        //初始速度
        particle.velocity = glm::vec2(0.0f, 1.0f);
    }
    void ParticleEmitter::DefaultParticleUpdate(Particle& p, float dt)
    {
        //默认更新方式
		p.position += p.velocity * dt;
		p.color.a -= dt * 2.5f;
    }
    //找到第一个没有被使用的粒子,同时记录最新被使用的粒子,其实就是将所有粒子遍历一边，找到对应的未被使用的粒子。
    uint64_t ParticleEmitter::FirstUnusedParticle() {
        for (uint64_t i = m_LastUsedParticle; i < m_MaxParticles; ++i) {
            if (m_Particles[i].life <= 0.0f) {
                m_LastUsedParticle = i;
                return i;
            }
        }
        for (uint64_t i = 0; i < m_LastUsedParticle; ++i) {
            if (m_Particles[i].life <= 0.0f) {
                m_LastUsedParticle = i;
                return i;
            }
        }
        //如果没找到，返回0。
        m_LastUsedParticle = 0;
        return 0;
    }

    //重激活粒子，重新调整生命值。
    void ParticleEmitter::RespawnParticle(Particle& particle) {
        //随机获取粒子值
        particle.random_seed = GenerateRandomFloat();
        //对种子进行初始化操作。
        m_ParticleInit(particle);
     }
    // 生成在[left, right]范围内的随机小数
    float ParticleEmitter::GenerateRandomFloat() {
        // 静态变量，只会初始化一次
        static std::random_device rd;  // 随机设备，用于生成随机种子
        static std::mt19937 gen(rd()); // 随机数引擎，使用随机设备生成随机种子
        std::uniform_real_distribution<float> dis(m_RandomLeft, m_RandomRight); // 均匀分布的随机数生成器
        return dis(gen); // 返回生成的随机数
    }
    void ParticleEmitter::Init()
    {
        //将一些预定义的
    }
}
