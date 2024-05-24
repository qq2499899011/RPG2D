#include "RPG2Dpch.h"
#include "AudioSystem.h"
#include "soloud.h"
#include "soloud_wav.h"
namespace RPG2D {
	AudioSystem::AudioSystem()
	{
		m_AudioEngine = CreateRef<SoLoud::Soloud>();
		if (m_AudioEngine->init() != SoLoud::SO_NO_ERROR) {
			RPG2D_CORE_ERROR("Can't Init m_AudioEngine");
		}
		else {
			RPG2D_CORE_INFO("AudioEngineInitSuccess!");
		}
	}
	AudioSystem::~AudioSystem()
	{
		m_AudioEngine->deinit();
	}
	void AudioSystem::Update(Timestep st)
	{
		//遍历所有带有声音组件
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		{
			auto group = m_Registry->view<AudioComponent>();
			for (auto entity : group)
			{
				auto& audio = group.get<AudioComponent>(entity);
				//音频组件
				if (!audio.play)continue;
				audio.play = false;
				audio.handle = m_AudioEngine->play(*audio.audio->GetRealAudioPtr(), audio.volume);
				m_AudioEngine->setLooping(audio.handle, audio.isLoop);
			}
		}
	}
	double AudioSystem::GetStreamTime(unsigned int handle) const
	{
		//如果有效，返回时间
		if (m_AudioEngine->isValidVoiceHandle(handle)) {
			return m_AudioEngine->getStreamTime(handle);
		}
		else {
			RPG2D_CORE_WARN("inValidVoiceHandle");
			return 0;
		}
	}
}
