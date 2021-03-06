#include "pch.h"
#include "CubeBox.h"

#include "Common\DirectXHelper.h"
#include "Content\DDSTextureLoader.h"
#include "WinUser.h"


using namespace D3D11Homework;

using namespace DirectX;
using namespace Windows::Foundation;

// 从文件中加载顶点和像素着色器，然后实例化立方体几何图形。
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



// 当窗口的大小改变时初始化视图参数。
void CubeBox::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// 这是一个简单的更改示例，当应用程序在纵向视图或对齐视图中时，可以进行此更改
	//。
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}
	
	// 请注意，OrientationTransform3D 矩阵在此处是后乘的，
	// 以正确确定场景的方向，使之与显示方向匹配。
	// 对于交换链的目标位图进行的任何绘制调用
	// 交换链呈现目标。对于到其他目标的绘制调用，
	// 不应应用此转换。

	// 此示例使用行主序矩阵利用右手坐标系。
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

	// 眼睛位于(0,0.7,5.5)，并沿着 Y 轴使用向上矢量查找点(0,-0.1,0)。	
}

// 每个帧调用一次，旋转model，并计算模型和视图矩阵。
void CubeBox::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{

		//保存累计变换
		XMMATRIX cumulativeMatrix = XMMatrixIdentity();

		// 初始化变换矩阵
		cumulativeMatrix *= XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		cumulativeMatrix *= XMMatrixTranslation(m_transform.x, m_transform.y, m_transform.z);
		// 具体的模型变换
		if (m_rotatable)
		{
			float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
			double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
			float radians = static_cast<float>(fmod(totalRotation, XM_2PI));
			cumulativeMatrix *= XMMatrixRotationY(radians);
		}

		// 设置灯光参数
		XMFLOAT4 vLightDirs = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		XMFLOAT4 vLightColors = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		// 准备将更新的模型矩阵传递到着色器
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



void CubeBox::StartTracking()
{
	m_tracking = true;
}

// 进行跟踪时，可跟踪指针相对于输出屏幕宽度的位置，从而让 3D 立方体围绕其 Y 轴旋转。
void CubeBox::TrackingUpdate(float positionX)
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

void CubeBox::StopTracking()
{
	m_tracking = false;
}

// 使用顶点和像素着色器呈现一个帧。
void CubeBox::Render()
{
	// 加载是异步的。仅在加载几何图形后才会绘制它。
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// 准备常量缓冲区以将其发送到图形设备。
	context->UpdateSubresource1(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0,
		0
	);

	// 每个顶点都是 VertexPCNT 结构的一个实例。
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
		DXGI_FORMAT_R16_UINT, // 每个索引都是一个 16 位无符号整数(short)。
		0
	);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// 附加我们的顶点着色器。
	context->VSSetShader( m_vertexShader.Get(), nullptr, 0);

	// 将常量缓冲区发送到图形设备。
	context->VSSetConstantBuffers1( 0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);

	// 附加我们的像素着色器。
	context->PSSetShader( m_pixelShader.Get(), nullptr, 0);
	context->PSSetShaderResources(0, 1, m_textureRV.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
	context->PSSetSamplers(0, 1, m_samplerLinear.GetAddressOf());
	// 绘制对象。
	context->DrawIndexed( m_indexCount, 0, 0);
}

void CubeBox::CreateDeviceDependentResources()
{
	// 通过异步方式加载着色器。
	auto loadVSTask = DX::ReadDataAsync(L"VertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"PixelShader.cso");

	// 加载顶点着色器文件之后，创建着色器和输入布局。
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

	// 加载像素着色器文件后，创建着色器和常量缓冲区。
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

	// 加载两个着色器后，创建网格。
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {

		// 加载网格顶点。每个顶点都有一个位置,法向量，纹理坐标。
		static const VertexPCNT cubeVertices[] =
		{
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },

			{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },

			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },

			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f) },

			{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT2(0.0f, 0.0f) },

			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT2(1.0f, 0.0f) },
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

		// 加载网格索引。每三个索引表示
		// 要在屏幕上呈现的三角形。
		// 例如: 0,2,1 表示顶点的索引
		// 顶点缓冲区中的索引为 0、2 和 1 的顶点构成了
		// 此网格的第一个三角形。
		static const unsigned short cubeIndices[] =
		{
			0,1,3,
			3,1,2,

			5,4,6,
			6,4,7,

			8,9,11,
			11,9,10,

			13,12,14,
			14,12,15,

			16,17,19,
			19,17,18,

			21,20,22,
			22,20,23
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
		
		// 加载纹理
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(
				m_deviceResources->GetD3DDevice(),
				L"Assets/Wood.dds",
				nullptr,
				&m_textureRV
			)
		);
	});

	// 加载立方体后，就可以呈现该对象了。
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
	m_samplerLinear.Reset();
	m_textureRV.Reset();
}