#pragma once

#include "RPG2D/Core/Base.h"
#include "RPG2D/Core/Layer.h"

#include <vector>

namespace RPG2D {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		//主要功能
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);


		//实现相关迭代器
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers; //储存layer的数组 [begin,begin+m_layerIndex]普通层  (begin+m_layerIndex,end)顶层
		unsigned int m_LayerInsertIndex = 0;//
	};

}