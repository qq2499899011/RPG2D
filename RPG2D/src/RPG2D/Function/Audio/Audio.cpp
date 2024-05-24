#include "RPG2Dpch.h"
#include "Audio.h"
#include "soloud.h"
#include "soloud_wav.h"

namespace RPG2D {
	Audio::Audio(const std::string path, const std::string name)
	{
		this->name = name;
		m_Audio = CreateRef<SoLoud::Wav>();
		Generate(path);
	}

	void Audio::Generate(const std::string path)
	{
		//加载音频
		if (m_Audio->load(path.c_str()) != SoLoud::SO_NO_ERROR) {
			// 加载失败处理
			RPG2D_CORE_ERROR("Can't Load m_Audio");
		}
		else {
			RPG2D_CORE_INFO("AudioLoadSuccess!,time={0}", m_Audio->getLength());
		}
	}

	Ref<SoLoud::Wav> Audio::GetRealAudio()
	{
		return m_Audio;
	}

	SoLoud::Wav* Audio::GetRealAudioPtr()
	{
		return m_Audio.get();
	}

	std::string Audio::GetName() const
	{
		return name;
	}

	double Audio::GetLength() const
	{
		return m_Audio->getLength();
	}
}
