#pragma once
#include "RPG2D/Core/Timestep.h"
namespace RPG2D {
	class UISystem
	{
	public:
		//≥ı ºªØ
		void Init();
		//Begin
		void Begin();
		//End
		void End();
		void Update(Timestep);
		void SetFontSize(float size) { m_FontSize = size; };
		float GetFontSize() { return m_FontSize; };
	private:
		float m_FontSize = 20.0f;
	};
}

