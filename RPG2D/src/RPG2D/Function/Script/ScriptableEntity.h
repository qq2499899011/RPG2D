#pragma once

#include "Entity.h"
#include "RPG2D/Core/Timestep.h"

//有一些函数,可以设置相应在update中执行的操作
namespace RPG2D {
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
		friend class ScriptSystem;
	};


}

