#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"

#include "Model.h"



namespace D3D11Homework
{
	
	// 此示例呈现器实例化一个基本渲染管道。
	class Plane :public Model
	{
	public:
		Plane(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		Plane(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 transform);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		bool IsTracking() { return m_tracking; }
	private:


	private:
		// 缓存的设备资源指针。
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// 立体几何的 Direct3D 资源。
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_samplerLinear;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureRV;

		// 立体几何的系统资源。
		uint32	m_indexCount;

		// 用于渲染循环的变量。
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;

		// 用于模型初始化的变量。
		DirectX::XMFLOAT3 m_scale;
		DirectX::XMFLOAT3 m_transform;

		// 附加参数
		bool	m_rotatable;	//是否旋转


	};
}

