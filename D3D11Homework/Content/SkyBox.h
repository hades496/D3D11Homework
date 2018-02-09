#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"
#include "SphereRenderer.h"
#include "Plane.h"
#include <DirectXMath.h>
#include "Model.h"
namespace D3D11Homework
{
	// 雪人
	class SkyBox : public Model
	{
	public:
		
		SkyBox(const std::shared_ptr<DX::DeviceResources>& deviceResources, float m_boxSize);
		void CreateWindowSizeDependentResources();
		void Update(DX::StepTimer const& timer, DirectX::XMFLOAT3);
		void Render();

		// 缓存的设备资源指针。
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		// IDeviceNotify
		virtual void ReleaseDeviceDependentResources();
		virtual void CreateDeviceDependentResources();
		void ChangeViewMatrix(const DirectX::XMMATRIX viewMatrix);
		void ChangeModelMatrix(const DirectX::XMMATRIX viewMatrix);
	private:
		// 附加参数
		bool	m_rotatable;	//是否旋转
		float	m_boxSize;		//天空盒大小
		// 天空盒的5个面
		std::unique_ptr<Plane> m_east;
		std::unique_ptr<Plane> m_north;
		std::unique_ptr<Plane> m_west;
		std::unique_ptr<Plane> m_south;
		std::unique_ptr<Plane> m_up;
	};
}

