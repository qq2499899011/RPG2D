#include "RPG2Dpch.h"
#include "AnimatiorController.h"
namespace RPG2D {
	AnimatiorController::AnimatiorController()
	{
		//随便设置一个animatior
	}

	void AnimatiorController::Update(Timestep ts)
	{
		//调用当前animatior的Update函数,并且返回当前texture2D；
		curAnimatior->Update(ts);
	}

	Ref<Texture2D> AnimatiorController::GetCurFrame()
	{
		return curAnimatior->GetCurFrame();
	}

	// 添加动画
	void AnimatiorController::AddAnimation(AnimationState state, Ref<Animatior> animatior) {
		animatiors[state] = animatior;
		curAnimatior = animatior;
	}

	// 设置当前基础状态
	void AnimatiorController::SetState(AnimationState newState) {
		//如果状态没变，不管
		if (newState == currentAnimationState)return;
		//保证当前状态有效
		if (animatiors.find(newState) == animatiors.end()) {
			RPG2D_CORE_ERROR("can't find animatiors");
			return;
		}
		//重置旧状态的时间
		curAnimatior->Reset();
		//设置状态,还有最大时间。
		currentAnimationState = newState;
		curAnimatior = animatiors[newState];
	}
}
