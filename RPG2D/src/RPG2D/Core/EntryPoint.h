#pragma once
#include"RPG2D/Core/Application.h"
#include"RPG2D/Core/LogSystem.h"
//程序入口
//需要声明外部的函数
#ifdef RPG2D_PLATFORM_WINDOWS
//依靠外部实现CreatApplication
extern RPG2D::Application* RPG2D::CreateApplication();
int main(int argc,char** argv) {
	//测试日志,初始化 
	RPG2D::Log::Init();
	RPG2D_CORE_INFO("Hello");
	RPG2D_WARN("client hello");
	//创建app并运行
	auto app = RPG2D::CreateApplication();
	RPG2D_WARN("www");
	//初始化并执行
	app->Init();
	app->Run();
	delete app;
	RPG2D_WARN("www1111111222");
}

#endif // RPG2D_PLATFORM_WINDOWS
