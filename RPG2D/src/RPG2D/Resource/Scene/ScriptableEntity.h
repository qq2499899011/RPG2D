#pragma once

#include "Entity.h"

//有一些函数,可以设置相应在update中执行的操作
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

