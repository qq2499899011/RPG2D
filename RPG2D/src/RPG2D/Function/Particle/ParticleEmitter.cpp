#include "RPG2Dpch.h"
#include "ParticleEmitter.h"
namespace RPG2D {
    //��������״̬
    void ParticleEmitter::Update(float dt) {
        //����Ѿ�С��0��ֱ�ӷ���
        if (m_IsTemp) {
            m_EmitterLife -= dt;
            if (m_EmitterLife <= 0.0f)return;
        }
        //����ʱ�䣬ȷ���Ƿ�������
        m_AccTime += dt;
        if (m_AccTime >= m_IntTime) {
            m_AccTime -= m_IntTime;
            Emit();
        }
        //������������
        for (uint64_t i = 0; i < m_MaxParticles; ++i) {
            Particle& p = m_Particles[i];
            p.life -= dt; //������������
            if (p.life > 0.0f) { //������ӻ�����,���ø��º���
                m_ParticleUpdate(p, dt);
            }
        }
    }
    //����һ������
    void ParticleEmitter::Emit() {
        uint64_t unusedParticle = FirstUnusedParticle();
        RespawnParticle(m_Particles[unusedParticle]);
    }
    void ParticleEmitter::DefaultParticleInit(Particle& particle)
    {
        //Ĭ�ϳ�ʼ��
        float random = particle.random_seed;
        //��ʼ�����λ��
        particle.position = glm::vec2(0.0f, 0.0f) + random;
        particle.color = glm::vec4(1.0f);
        particle.life = m_ParticleLife;
        //��ʼ�ٶ�
        particle.velocity = glm::vec2(0.0f, 1.0f);
    }
    void ParticleEmitter::DefaultParticleUpdate(Particle& p, float dt)
    {
        //Ĭ�ϸ��·�ʽ
		p.position += p.velocity * dt;
		p.color.a -= dt * 2.5f;
    }
    //�ҵ���һ��û�б�ʹ�õ�����,ͬʱ��¼���±�ʹ�õ�����,��ʵ���ǽ��������ӱ���һ�ߣ��ҵ���Ӧ��δ��ʹ�õ����ӡ�
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
        //���û�ҵ�������0��
        m_LastUsedParticle = 0;
        return 0;
    }

    //�ؼ������ӣ����µ�������ֵ��
    void ParticleEmitter::RespawnParticle(Particle& particle) {
        //�����ȡ����ֵ
        particle.random_seed = GenerateRandomFloat();
        //�����ӽ��г�ʼ��������
        m_ParticleInit(particle);
     }
    // ������[left, right]��Χ�ڵ����С��
    float ParticleEmitter::GenerateRandomFloat() {
        // ��̬������ֻ���ʼ��һ��
        static std::random_device rd;  // ����豸�����������������
        static std::mt19937 gen(rd()); // ��������棬ʹ������豸�����������
        std::uniform_real_distribution<float> dis(m_RandomLeft, m_RandomRight); // ���ȷֲ��������������
        return dis(gen); // �������ɵ������
    }
    void ParticleEmitter::Init()
    {
        //��һЩԤ�����
    }
}
