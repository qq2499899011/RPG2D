#pragma once

#include "Entity.h"

//��һЩ����,����������Ӧ��update��ִ�еĲ���
namespace RPG2D {
	class ScriptableEntity:public Entity
	{
	public:
		virtual ~ScriptableEntity() {}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		friend class Scene;
	};

}

