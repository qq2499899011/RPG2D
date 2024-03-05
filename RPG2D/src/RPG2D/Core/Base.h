#pragma once
#include <memory>
#define RPG2D_CORE_ASSERT(...) //TODO:先什么都不做。放在这里之后会删除

#define BIT(x) (1 << x)

//任意参数，返回类型任意的函数。lamada表达式根据fn的返回值类型和参数类型推导生成的匿名函数的返回类型和参数类型。
//对调用此函数的对象进行捕获。
#define RPG2D_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace RPG2D {

	//创建函数指针的工厂函数
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)//编译时求值，提高代码运行效率
	{
		return std::make_unique<T>(std::forward<Args>(args)...);//完美转发参数，并根据参数构造T。
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}


