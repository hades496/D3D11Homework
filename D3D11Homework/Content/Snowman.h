#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"
#include "SphereRenderer.h"
#include <DirectXMath.h>
#include "Model.h"
namespace D3D11Homework
{
	// 雪人
	class Snowman : public Model
	{
	public:
		Snowman(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		Snowman(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 transform);
		void CreateWindowSizeDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

		// 缓存的设备资源指针。
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		// IDeviceNotify
		virtual void ReleaseDeviceDependentResources();
		virtual void CreateDeviceDependentResources();
		void ChangeViewMatrix(const DirectX::XMVECTORF32 eye, const DirectX::XMVECTORF32 at, const DirectX::XMVECTORF32 up);
		void ChangeViewMatrix(const DirectX::XMMATRIX viewMatrix);
	private:
		// 附加参数
		bool	m_rotatable;	//是否旋转

		// 两个球面模型
		std::unique_ptr<SphereRenderer> m_UpperSphere;
		std::unique_ptr<SphereRenderer> m_LowerSphere;
	};
}

