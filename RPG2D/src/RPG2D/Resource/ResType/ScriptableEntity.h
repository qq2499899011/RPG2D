#pragma once
#include "Entity.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Core/LogSystem.h"
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
		virtual void OnCollisionBegin(Entity other) {
			//输出碰撞
			//RPG2D_CORE_INFO("{0}:contact {1}",m_Entity.GetName(),other.GetName());
		}
	protected:
		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate(Timestep ts) = 0;
		Entity m_Entity;
	private:
		friend class Scene;
		friend class ScriptSystem;
	};


}

