#pragma once
#include "RPG2D/Core/Timestep.h"
namespace RPG2D {
	class AnimationSystem
	{
		//���б���animatorController���й�����ȡSpriteRender�����animatiorController�����update�����texture2D�ĸ��¡�
		//����spriteRenderer��Ӧ��texture
	public:
		//��ʼ����ʲô��
		void Init();
		void Update(Timestep ts);
	};
}
