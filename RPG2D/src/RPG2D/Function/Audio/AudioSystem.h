#pragma once
#include "RPG2D/Core/Base.h"
#include "RPG2D/Core/Timestep.h"
namespace SoLoud {
	class Soloud;
}
namespace RPG2D {
	class AudioSystem
	{
	public:
		AudioSystem();
		~AudioSystem();
		void Update(Timestep st);
		double GetStreamTime(unsigned int handle)const;//��ȡʣ��ʱ��
	private:
		//��Ƶ����
		Ref<SoLoud::Soloud> m_AudioEngine;
	};
}

