#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "Model.h"
#include <vector>

namespace D3D11Homework
{
	// 场景渲染
	class SceneRenderer
	{
	public:
		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		SceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, void* start());
		void Update(DX::StepTimer const& timer);
		void Render();
		void Add(std::unique_ptr<Model>);
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		void ReleaseDeviceDependentResources();
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		
	private:
		std::vector<std::unique_ptr<Model>> m_modelManager; //模型管理器
	};
}

