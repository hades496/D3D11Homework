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
		float Pad; // ռλ���һ��float���������ǾͿ������ù�Դ�����ˡ�
	};

	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		// �����4Dʸ��: (Position, Range)
		DirectX::XMFLOAT3 Position;
		float Range;

		// �����4Dʸ��: (A0, A1, A2, Pad)
		DirectX::XMFLOAT3 Att;
		float Pad; // ռλ���һ��float���������ǾͿ������ù�Դ�����ˡ�
	};

	struct SpotLight
	{
		SpotLight() { ZeroMemory(this, sizeof(this)); }

		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;

		// �����4Dʸ��: (Position, Range)
		DirectX::XMFLOAT3 Position;
		float Range;

		// �����4Dʸ��: (Direction, Spot)
		DirectX::XMFLOAT3 Direction;
		float Spot;

		// �����4Dʸ��: (Att, Pad)
		DirectX::XMFLOAT3 Att;
		float Pad; // ռλ���һ��float�����������ǾͿ������ù�Դ�����ˡ�
	};

	// ����
	struct Material
	{
		DirectX::XMFLOAT4 Ambient;
		DirectX::XMFLOAT4 Diffuse;
		DirectX::XMFLOAT4 Specular;  //  w����Ϊ�߹�ǿ��
		DirectX::XMFLOAT4 Reflect;
	};

}
