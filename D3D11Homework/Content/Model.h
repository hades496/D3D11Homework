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
		virtual void Update(DX::StepTimer const& timer, DirectX::XMFLOAT3);
		virtual void Render();
		virtual void ReleaseDeviceDependentResources();
		virtual void CreateWindowSizeDependentResources();
		
		virtual void ChangeViewMatrix(const DirectX::XMMATRIX viewMatrix);
		virtual void ChangeModelMatrix(const DirectX::XMMATRIX viewMatrix);
		ModelViewProjectionConstantBuffer	m_constantBufferData;
	};
}

