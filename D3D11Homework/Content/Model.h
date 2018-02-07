#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"
#include "..\Common\LightHelper.h"

namespace D3D11Homework
{
	// 模型基类
	class Model
	{
	public:
		Model();
		virtual ~Model();
		virtual void CreateDeviceDependentResources();
		virtual void Update(DX::StepTimer const& timer);
		virtual void Render();
		virtual void ReleaseDeviceDependentResources();
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		
		virtual void CreateWindowSizeDependentResources();
	};
}

