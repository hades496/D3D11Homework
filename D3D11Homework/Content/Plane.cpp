#include "pch.h"
#include "Plane.h"

#include "Common\DirectXHelper.h"
#include "Content\DDSTextureLoader.h"
#include "WinUser.h"


using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;

// ���ļ��м��ض����������ɫ����Ȼ��ʵ���������弸��ͼ�Ρ�
Plane::Plane(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

Plane::Plane(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, XMFLOAT3 scale, XMFLOAT3 transform,UINT dir) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources),
	m_rotatable(rotatable),
	m_scale(scale),
	m_transform(transform),
	m_direct(dir)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// �����ڵĴ�С�ı�ʱ��ʼ����ͼ������
void Plane::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// ����һ���򵥵ĸ���ʾ������Ӧ�ó�����������ͼ�������ͼ��ʱ�����Խ��д˸���
	//��
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// ��ע�⣬OrientationTransform3D �����ڴ˴��Ǻ�˵ģ�
	// ����ȷȷ�������ķ���ʹ֮����ʾ����ƥ�䡣
	// ���ڽ�������Ŀ��λͼ���е��κλ��Ƶ���
	// ����������Ŀ�ꡣ���ڵ�����Ŀ��Ļ��Ƶ��ã�
	// ��ӦӦ�ô�ת����

	// ��ʾ��ʹ�����������������������ϵ��
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
	);	
}

// ÿ��֡����һ�Σ���תmodel��������ģ�ͺ���ͼ����
void Plane::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{

		//�����ۼƱ任
		XMMATRIX cumulativeMatrix = XMMatrixIdentity();
		// ��ʼ���任����
		cumulativeMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		cumulativeMatrix *= XMMatrixTranslation(m_transform.x, m_transform.y, m_transform.z);
		
		// ���õƹ����
		XMFLOAT4 vLightDirs = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		XMFLOAT4 vLightColors = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// ׼�������µ�ģ�;��󴫵ݵ���ɫ��
		XMStoreFloat4x4(
			&m_constantBufferData.model,
			XMMatrixTranspose(
				cumulativeMatrix
			)
		);
		m_constantBufferData.vLightColor = vLightColors;
		m_constantBufferData.vLightDir = vLightDirs;

	}
}

// ÿ��֡����һ�Σ���תmodel��������ģ�ͺ���ͼ����
void Plane::Update(DX::StepTimer const& timer, XMFLOAT3 cameraPos)
{
	if (!m_tracking)
	{

		//�����ۼƱ任
		XMMATRIX cumulativeMatrix = XMMatrixIdentity();
		if (m_direct == 1) //��ת������
		{
			cumulativeMatrix *= XMMatrixRotationY(-XM_PIDIV2);
			cumulativeMatrix *= XMMatrixRotationZ(XM_PIDIV2); 
		}
		if (m_direct == 2)//��ת���ϲ�
		{
			cumulativeMatrix *= XMMatrixRotationY(XM_PI);
			cumulativeMatrix *= XMMatrixRotationX(-XM_PIDIV2); 
		}
		if (m_direct == 3)//��ת������
		{
			cumulativeMatrix *= XMMatrixRotationY(XM_PIDIV2);
			cumulativeMatrix *= XMMatrixRotationZ(-XM_PIDIV2); 
		}
		if (m_direct == 4)//��ת������
		{
			cumulativeMatrix *= XMMatrixRotationX(XM_PIDIV2); 
		}
		if (m_direct == 5) //��ת������
		{
			cumulativeMatrix *= XMMatrixRotationY(-XM_PI);
			cumulativeMatrix *= XMMatrixRotationX(XM_PI);
		}

		// ��ʼ���任����
		cumulativeMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		cumulativeMatrix *= XMMatrixTranslation(m_transform.x, m_transform.y, m_transform.z);
		if (m_direct > 0)
		{
			cumulativeMatrix *= XMMatrixTranslation(cameraPos.x, cameraPos.y, cameraPos.z);
		}
			
		// ׼�������µ�ģ�;��󴫵ݵ���ɫ��
		XMStoreFloat4x4(
			&m_constantBufferData.model,
			XMMatrixTranspose(
				cumulativeMatrix
			)
		);

	}
}



void Plane::StartTracking()
{
	m_tracking = true;
}

// ���и���ʱ���ɸ���ָ������������Ļ��ȵ�λ�ã��Ӷ��� 3D ������Χ���� Y ����ת��
void Plane::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		XMMATRIX rotation_Matrix = XMMatrixRotationY(radians);
		XMStoreFloat4x4(
			&m_constantBufferData.model,
			XMMatrixTranspose(
				rotation_Matrix
			)
		);
	}
}

void Plane::StopTracking()
{
	m_tracking = false;
}

// ʹ�ö����������ɫ������һ��֡��
void Plane::Render()
{
	// �������첽�ġ����ڼ��ؼ���ͼ�κ�Ż��������
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// ׼�������������Խ��䷢�͵�ͼ���豸��
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
	);

	// ÿ�����㶼�� VertexPCNT �ṹ��һ��ʵ����
	UINT stride = sizeof(VertexPCNT);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // ÿ����������һ�� 16 λ�޷�������(short)��
		0
	);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// �������ǵĶ�����ɫ����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);

	// ���������������͵�ͼ���豸��
	context->VSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);

	// �������ǵ�������ɫ����
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_textureRV.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	context->PSSetSamplers(0, 1, m_samplerLinear.GetAddressOf());
	// ���ƶ���
	context->DrawIndexed(m_indexCount, 0, 0);
}

void Plane::CreateDeviceDependentResources()
{
	// ͨ���첽��ʽ������ɫ����
	auto loadVSTask = DX::ReadDataAsync(
		m_direct == 0 ?
		L"VertexShader.cso" :
		L"SkyBoxVertexShader.cso"
	);
	auto loadPSTask = DX::ReadDataAsync(
		m_direct == 0?
		L"PixelShader.cso":
		L"SkyBoxPixelShader.cso"
	);

	// ���ض�����ɫ���ļ�֮�󣬴�����ɫ�������벼�֡�
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
			)
		);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
			)
		);
	});

	// ����������ɫ���ļ��󣬴�����ɫ���ͳ�����������
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
			)
		);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
			)
		);

		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateSamplerState(
				&sampDesc,
				&m_samplerLinear
			)
		);

	});

	// ����������ɫ���󣬴�������
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {

		// �������񶥵㡣ÿ�����㶼��һ��λ��,���������������ꡣ
		static const VertexPCNT cubeVertices[] =
		{
			{ XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0, 0.0f) },
			{ XMFLOAT3(-0.5f, 0.0f, 0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(0.5f, 0.0f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(0.5f, 0.0f, 0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) },

		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = cubeVertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
			)
		);

		// ��������������ÿ����������ʾ
		// Ҫ����Ļ�ϳ��ֵ������Ρ�
		// ����: 0,2,1 ��ʾ���������
		// ���㻺�����е�����Ϊ 0��2 �� 1 �Ķ��㹹����
		// ������ĵ�һ�������Ρ�
		static const unsigned short cubeIndices[] =
		{
			0,2,3,
			1,0,3,
		};

		m_indexCount = ARRAYSIZE(cubeIndices);

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = cubeIndices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
			)
		);

		// ��������
		auto ddsFile = L"Assets/Grass.dds";
		if (m_direct == 1) ddsFile = L"Assets/SkyBox_east.dds";
		if (m_direct == 2) ddsFile = L"Assets/SkyBox_south.dds";
		if (m_direct == 3) ddsFile = L"Assets/SkyBox_west.dds";
		if (m_direct == 4) ddsFile = L"Assets/SkyBox_north.dds";
		if (m_direct == 5) ddsFile = L"Assets/SkyBox_up.dds";

		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(
				m_deviceResources->GetD3DDevice(),
				ddsFile,
				nullptr,
				&m_textureRV
			)
		);
	});

	// ����������󣬾Ϳ��Գ��ָö����ˡ�
	createCubeTask.then([this]() {
		m_loadingComplete = true;
	});
}

void Plane::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
	m_samplerLinear.Reset();
	m_textureRV.Reset();
}