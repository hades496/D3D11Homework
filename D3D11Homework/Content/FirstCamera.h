#pragma once

#include<InputClass.h>
using namespace DirectX;
namespace D3D11Homework
{
	class FirstCamera
	{
	private:

		//���������ռ��λ�úͷ�������
		XMFLOAT4 mPosition;
		XMFLOAT4 mUp;
		XMFLOAT4 mLook;
		int mouseXOffset, mouseYOffset;
		float rotateY;

		//�����û�����
		InputClass Listener;
		float m_movePerSecond = 0.5f;

	public:
		FirstCamera();
		FirstCamera(const FirstCamera& other);
		~FirstCamera();

	public:
		//ͨ�����������ռ��λ�ã�Ŀ��㣬�Լ�����������������任����
		XMMATRIX LookAt();

		//�����ƶ�(�����������ƶ�)
		void Strafe(float d);

		//ǰ���ƶ�(����LookAt�����ƶ�)
		void Walk(float d);

		//�����ƶ�(��������ռ��Y���ƶ�)
		void UpDown(float d);

		//���������������ת
		void Pitch(float angle);

		//������ռ��Y�������ת,ע����ת�Ƕ�Ӧ���ڳ�ʼ���������90��֮��,������������������(-90.0f<=RotateAngle<=90.0f)
		void RotateY(float angle);

		//ÿ֡�������λ��
		void Update();
	};
}