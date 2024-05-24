#pragma once
namespace SoLoud {
	class Wav;
}
namespace RPG2D {
	class Audio
	{
	public:
		Audio(const std::string,const std::string);
		~Audio() = default;
		void Generate(const std::string);
		Ref<SoLoud::Wav> GetRealAudio();
		SoLoud::Wav* GetRealAudioPtr();
		std::string GetName()const;
		double GetLength()const;
	private:
		std::string name;
		Ref<SoLoud::Wav> m_Audio;
	};
}

