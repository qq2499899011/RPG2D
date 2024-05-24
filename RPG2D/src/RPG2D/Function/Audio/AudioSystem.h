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
		double GetStreamTime(unsigned int handle)const;//获取剩余时间
	private:
		//音频引擎
		Ref<SoLoud::Soloud> m_AudioEngine;
	};
}

