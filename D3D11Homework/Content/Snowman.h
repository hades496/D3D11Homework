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
	// ѩ��
	class Snowman : public Model
	{
	public:
		Snowman(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		Snowman(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 transform);
		void CreateWindowSizeDependentResources();
		void Start();
		void Update(DX::StepTimer const& timer);
		void Render();

		// ������豸��Դָ�롣
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		// IDeviceNotify
		virtual void ReleaseDeviceDependentResources();
		virtual void CreateDeviceDependentResources();

	private:
		// ���Ӳ���
		bool	m_rotatable;	//�Ƿ���ת

		// ��������ģ��
		std::unique_ptr<SphereRenderer> m_UpperSphere;
		std::unique_ptr<SphereRenderer> m_LowerSphere;
	};
}

