#include "RPG2Dpch.h"
#include "AnimatiorController.h"
namespace RPG2D {
	AnimatiorController::AnimatiorController()
	{
		//�������һ��animatior
	}

	void AnimatiorController::Update(Timestep ts)
	{
		//���õ�ǰanimatior��Update����,���ҷ��ص�ǰtexture2D��
		curAnimatior->Update(ts);
	}

	Ref<Texture2D> AnimatiorController::GetCurFrame()
	{
		return curAnimatior->GetCurFrame();
	}

	// ��Ӷ���
	void AnimatiorController::AddAnimation(AnimationState state, Ref<Animatior> animatior) {
		animatiors[state] = animatior;
		curAnimatior = animatior;
	}

	// ���õ�ǰ����״̬
	void AnimatiorController::SetState(AnimationState newState) {
		//���״̬û�䣬����
		if (newState == currentAnimationState)return;
		//��֤��ǰ״̬��Ч
		if (animatiors.find(newState) == animatiors.end()) {
			RPG2D_CORE_ERROR("can't find animatiors");
			return;
		}
		//���þ�״̬��ʱ��
		curAnimatior->Reset();
		//����״̬,�������ʱ�䡣
		currentAnimationState = newState;
		curAnimatior = animatiors[newState];
	}
}
