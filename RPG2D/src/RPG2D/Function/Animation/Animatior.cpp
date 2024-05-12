#include "RPG2Dpch.h"
#include "Animatior.h"
namespace RPG2D {
	Ref<Texture2D> Animatior::GetCurFrame()
	{
		return curTexture;
	}
    // 添加纹理
    void Animatior::AddFrame(Timestep ts, Ref<Texture2D> texture){
        //还要记录最大时间。
        if (ts > maxTime-0.1f)maxTime = ts+0.1f;
        frames[ts] = texture;
    }
    // 更新时间
    void Animatior::Update(Timestep deltaTime) {
        //更新时间
        currentTime = currentTime + deltaTime;
        if (currentTime > maxTime)currentTime = currentTime - maxTime;
        //根据最新时间确定纹理对象
        auto it = frames.upper_bound(currentTime); // 查找大于当前时间的第一个元素的迭代器
        if (it != frames.begin())--it; // 将迭代器退回到小于等于当前时间的最大值
        curTexture = it->second;
    }
    //重置时间
    void Animatior::Reset() 
    {
        currentTime = 0;
    }
}
