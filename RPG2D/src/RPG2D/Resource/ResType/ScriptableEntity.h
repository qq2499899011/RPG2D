#pragma once
#include "Entity.h"
#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Core/LogSystem.h"
//��һЩ����,����������Ӧ��update��ִ�еĲ���
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
		virtual bool OnCollisionBegin(Entity other) {
			//�����ײ
			//RPG2D_CORE_INFO("{0}:contact {1}",m_Entity.GetName(),other.GetName());
			return true;
		}		
		virtual bool OnCollision(Entity other) {
			//��ײ������
			return true;
		}
		virtual bool OnCollisionEnd(Entity other) {
			//�����ײ
			//RPG2D_CORE_INFO("{0}:contact end{1}",m_Entity.GetName(),other.GetName());
			return true;
		}
	protected:
		virtual void OnStart() {
			
		};
		virtual void OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate(Timestep ts) = 0;
		Entity m_Entity;
	private:
		friend class Scene;
		friend class ScriptSystem;
	};


}

