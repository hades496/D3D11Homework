#pragma once
#include "DirectXHelper.h"

namespace D3D11Homework
{
	struct DirectionalLight
	{
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;
		DirectX::XMFLOAT3 Direction;
		float Pad; // 占位最后一个float，这样我们就可以设置光源数组了。
	};

	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		// 打包到4D矢量: (Position, Range)
		DirectX::XMFLOAT3 Position;
		float Range;

		// 打包到4D矢量: (A0, A1, A2, Pad)
		DirectX::XMFLOAT3 Att;
		float Pad; // 占位最后一个float，这样我们就可以设置光源数组了。
	};

	struct SpotLight
	{
		SpotLight() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		// 打包到4D矢量: (Position, Range)
		DirectX::XMFLOAT3 Position;
		float Range;

		// 打包到4D矢量: (Direction, Spot)
		DirectX::XMFLOAT3 Direction;
		float Spot;

		// 打包到4D矢量: (Att, Pad)
		DirectX::XMFLOAT3 Att;
		float Pad; // 占位最后一个float，，这样我们就可以设置光源数组了。
	};

	// 材质
	struct Material
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;  //  w分量为高光强度
		DirectX::XMFLOAT4 Reflect;
	};

}
