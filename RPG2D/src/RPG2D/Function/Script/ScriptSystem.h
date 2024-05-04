#pragma once
#include "RPG2D/Core/Timestep.h"
namespace RPG2D {
	class ScriptSystem
	{
	public:
		ScriptSystem();
		//更新
		void Update(Timestep ts);
		//初始化
		void Init();

	};
}
