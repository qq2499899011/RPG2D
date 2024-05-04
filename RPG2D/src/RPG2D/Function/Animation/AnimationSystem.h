#pragma once
#include "RPG2D/Core/Timestep.h"
namespace RPG2D {
	class AnimationSystem
	{
		//所有遍历animatorController进行管理，获取SpriteRender组件和animatiorController组件，update后进行texture2D的更新。
		//设置spriteRenderer对应的texture
	public:
		//初始化干什么？
		void Init();
		void Update(Timestep ts);
	};
}
