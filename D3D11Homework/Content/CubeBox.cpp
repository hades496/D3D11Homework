#include "pch.h"
#include "CubeBox.h"

#include "..\Common\DirectXHelper.h"



using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;

// ���ļ��м��ض����������ɫ����Ȼ��ʵ���������弸��ͼ�Ρ�
CubeBox::CubeBox(const std::shared_ptr<DX::DeviceResources>& deviceResources):
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)	
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

CubeBox::CubeBox(const std::shared_ptr<DX::DeviceResources>& deviceResources, const bool rotatable, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 transform) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources),
	m_rotatable(rotatable),
	m_scale(scale),
	m_transform(transform)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}



// �����ڵĴ�С�ı�ʱ��ʼ����ͼ������
void CubeBox::CreateWindowSizeDependentResources()
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

	// �۾�λ��(0,0.7,5.5)�������� Y ��ʹ������ʸ�����ҵ�(0,-0.1,0)��
	static const XMVECTORF32 eye = { 0.0f, 1.7f, 5.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// ÿ��֡����һ�Σ���תmodel��������ģ�ͺ���ͼ����
void CubeBox::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{

		//�����ۼƱ任
		XMMATRIX cumulativeMatrix = XMMatrixIdentity();

		// ��ʼ���任����
		cumulativeMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		cumulativeMatrix *= XMMatrixTranslation(m_transform.x, m_transform.y, m_transform.z);

		// ����ı任
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;

		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));
		if (m_rotatable)
		{
			
			cumulativeMatrix *= XMMatrixRotationY(radians);
		}
			

		// ����ģ��	
		ModelUpdate(cumulativeMatrix);
	}
}

// �úϱ任�������ģ��
void CubeBox::ModelUpdate(CXMMATRIX cumulativeMatrix)
{
	// ׼�������µ�ģ�;��󴫵ݵ���ɫ��
	XMStoreFloat4x4(
		&m_constantBufferData.model,
		XMMatrixTranspose(
			cumulativeMatrix
		)
	);
}


void CubeBox::StartTracking()
{
	m_tracking = true;
}

// ���и���ʱ���ɸ���ָ������������Ļ��ȵ�λ�ã��Ӷ��� 3D ������Χ���� Y ����ת��
void CubeBox::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		XMMATRIX rotation_Matrix = XMMatrixRotationY(radians);
		ModelUpdate(rotation_Matrix);
	}
}

void CubeBox::StopTracking()
{
	m_tracking = false;
}

// ʹ�ö����������ɫ������һ��֡��
void CubeBox::Render()
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

	// ÿ�����㶼�� VertexPositionColor �ṹ��һ��ʵ����
	UINT stride = sizeof(VertexPositionColor);
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
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
	);

	// ���������������͵�ͼ���豸��
	context->VSSetConstantBuffers1(
		0,
		1,
		m_constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	// �������ǵ�������ɫ����
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
	);

	// ���ƶ���
	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}

void CubeBox::CreateDeviceDependentResources()
{
	// ͨ���첽��ʽ������ɫ����
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

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
	});

	// ����������ɫ���󣬴�������
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {

		// �������񶥵㡣ÿ�����㶼��һ��λ�ú�һ����ɫ��
		static const VertexPositionColor cubeVertices[] =
		{
			{ XMFLOAT3(1.0f, -0.5f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(1.0f, -0.5f, 2.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(1.0f,  0.5f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(1.0f,  0.5f, 2.0f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(2.0f, -0.5f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(2.0f, -0.5f, 2.0f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(2.0f,  0.5f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(2.0f,  0.5f, 2.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
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
			0,2,1, // -x
			1,2,3,

			4,5,6, // +x
			5,7,6,

			0,1,5, // -y
			0,5,4,

			2,6,7, // +y
			2,7,3,

			0,4,6, // -z
			0,6,2,

			1,3,7, // +z
			1,7,5,
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
	});

	// ����������󣬾Ϳ��Գ��ָö����ˡ�
	createCubeTask.then([this]() {
		m_loadingComplete = true;
	});
}

void CubeBox::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}