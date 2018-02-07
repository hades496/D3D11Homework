#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"

#include "Model.h"

namespace D3D11Homework
{
	// ��ʾ��������ʵ����һ��������Ⱦ�ܵ���
	class CubeBox:public Model
	{
	public:
		CubeBox(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		CubeBox(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 transform);
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
		// ������豸��Դָ�롣
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// ���弸�ε� Direct3D ��Դ��
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>	m_samplerLinear;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureRV;

		// ���弸�ε�ϵͳ��Դ��
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// ������Ⱦѭ���ı�����
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;

		// ����ģ�ͳ�ʼ���ı�����
		DirectX::XMFLOAT3 m_scale;
		DirectX::XMFLOAT3 m_transform;

		// ���Ӳ���
		bool	m_rotatable;	//�Ƿ���ת
		

	};
}

