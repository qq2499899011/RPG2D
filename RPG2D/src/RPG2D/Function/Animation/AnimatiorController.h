#pragma once
#include "Animatior.h"
#include "RPG2D/Core/Timestep.h"
//状态+对应动画
namespace RPG2D {
	// 定义动画状态
	enum class AnimationState {
		Idle,
		Walking,
		Running,
		Jumping,
	};
	class AnimatiorController
	{
		//有一系列的animator组件。
		//组件与animator对应。
	public:
		AnimatiorController();
		// 添加动画
		void AddAnimation(AnimationState state, Ref<Animatior> animatior);
		// 设置当前基础状态
		void SetState(AnimationState newState);
		//Update
		void Update(Timestep ts);
		//获取当前指向的纹理
		Ref<Texture2D> GetCurFrame();

	private:
		AnimationState currentAnimationState;
		Ref<Animatior> curAnimatior;
		std::unordered_map<AnimationState, Ref<Animatior>> animatiors;
	};
}

