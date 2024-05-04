#pragma once
#include <map>
#include "RPG2D/Function/Renderer/Texture2D.h"
//时间与对应的texture
namespace RPG2D {
	class Animatior
	{
    public:
        //重置当前时间
        void Reset();
        // 添加纹理
        void AddFrame(Timestep ts, Ref<Texture2D> texture);
        // 更新时间
        void Update(Timestep deltaTime);
        Ref<Texture2D> GetCurFrame();
    private:
        Timestep currentTime = 0.0f;
        Timestep maxTime = 0.0f;
        Ref<Texture2D> curTexture;
        std::map<Timestep, Ref<Texture2D>> frames;
	};
}

