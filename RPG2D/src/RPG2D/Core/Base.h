#pragma once
#include <memory>
#define RPG2D_CORE_ASSERT(...) //TODO:��ʲô����������������֮���ɾ��

#define BIT(x) (1 << x)

//���������������������ĺ�����lamada���ʽ����fn�ķ���ֵ���ͺͲ��������Ƶ����ɵ����������ķ������ͺͲ������͡�
//�Ե��ô˺����Ķ�����в���
#define RPG2D_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace RPG2D {

	//��������ָ��Ĺ�������
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)//����ʱ��ֵ����ߴ�������Ч��
	{
		return std::make_unique<T>(std::forward<Args>(args)...);//����ת�������������ݲ�������T��
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}


