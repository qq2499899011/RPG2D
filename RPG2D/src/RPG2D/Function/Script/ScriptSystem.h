#pragma once
#include "RPG2D/Core/Timestep.h"
namespace RPG2D {
	class ScriptSystem
	{
	public:
		ScriptSystem();
		//����
		void Update(Timestep ts);
		//��ʼ��
		void Init();
		//�ͷ���Դ
		void DeInit();

	};
}
