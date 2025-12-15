#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel
{
	LayerStack::LayerStack()
	{
		
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			delete layer;
		}
	}

	//在mLayerInsert迭代器的位置构造
	void LayerStack::PushLayer(Layer* layer)
	{
		mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
		mLayerInsertIndex++;
	}

	//添加
	void LayerStack::PushOverlay(Layer* overlay)
	{
		//在末端插入元素。原地构造，避免拷贝
		mLayers.emplace_back(overlay);
	}

	//移除并改变mLayerInsert迭代器位置
	void LayerStack::PopLayer(Layer* layer)
	{
		//遍历查找layer，若找到则返回迭代器指针
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
			mLayerInsertIndex--;
		}
	}

	//移除
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
		if (it != mLayers.end())
		{
			mLayers.erase(it);
		}
	}
}