#pragma once
#include "Animatior.h"
#include "RPG2D/Core/Timestep.h"
//״̬+��Ӧ����
namespace RPG2D {
	// ���嶯��״̬
	enum class AnimationState {
		Idle,
		Walking,
		Running,
		Jumping,
	};
	class AnimatiorController
	{
		//��һϵ�е�animator�����
		//�����animator��Ӧ��
	public:
		AnimatiorController();
		// ��Ӷ���
		void AddAnimation(AnimationState state, Ref<Animatior> animatior);
		// ���õ�ǰ����״̬
		void SetState(AnimationState newState);
		//Update
		void Update(Timestep ts);
		//��ȡ��ǰָ�������
		Ref<Texture2D> GetCurFrame();

	private:
		AnimationState currentAnimationState;
		Ref<Animatior> curAnimatior;
		std::unordered_map<AnimationState, Ref<Animatior>> animatiors;
	};
}

