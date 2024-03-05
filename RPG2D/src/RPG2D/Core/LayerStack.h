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

		//��Ҫ����
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);


		//ʵ����ص�����
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers; //����layer������ [begin,begin+m_layerIndex]��ͨ��  (begin+m_layerIndex,end)����
		unsigned int m_LayerInsertIndex = 0;//
	};

}