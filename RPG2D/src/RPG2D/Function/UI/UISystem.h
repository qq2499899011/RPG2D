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
	private:
		bool show_demo_window = true;
		bool show_another_window = false;
		int counter = 0;
	};
}

