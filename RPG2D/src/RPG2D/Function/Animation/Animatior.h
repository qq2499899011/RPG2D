#pragma once
#include <map>
#include "RPG2D/Function/Renderer/Texture2D.h"
//ʱ�����Ӧ��texture
namespace RPG2D {
	class Animatior
	{
    public:
        //���õ�ǰʱ��
        void Reset();
        // �������
        void AddFrame(Timestep ts, Ref<Texture2D> texture);
        // ����ʱ��
        void Update(Timestep deltaTime);
        Ref<Texture2D> GetCurFrame();
    private:
        Timestep currentTime = 0.0f;
        Timestep maxTime = 0.0f;
        Ref<Texture2D> curTexture;
        std::map<Timestep, Ref<Texture2D>> frames;
	};
}

