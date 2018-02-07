#pragma once

#ifndef  _FIRST_CAMERA_CLASS_H
#define _FIRST_CAMERA_CLASS_H
//������װ�������λ�ú���ת��
#include<Windows.h>

using namespace DirectX;
namespace D3D11Homework
{
	class FirstCamera
	{
	private:
		//���������ռ��λ�úͷ�������
		XMFLOAT3 mPosition;
		XMFLOAT3 mRight;
		XMFLOAT3 mUp;
		XMFLOAT3 mLook;

		//��������任����
		XMMATRIX mViewMatrix;
		XMMATRIX mBaseViewMatrix;

	public:
		FirstCamera();
		FirstCamera(const FirstCamera& other);
		~FirstCamera();

	public:
		//���λ��
		void SetPosition(float x, float y, float z);
		void SetPosition(const XMFLOAT3& v);
		XMFLOAT3 GetPosition()const;
		XMVECTOR GetPositionXM()const;

		//��ȡ����Ļ�������(Up,Look,Right)
		XMFLOAT3 GetUp()const;
		XMVECTOR GetUpXM()const;
		XMFLOAT3 GetLook()const;
		XMVECTOR GetLookXM()const;
		XMFLOAT3 GetRight()const;
		XMVECTOR GetRightXM()const;

		//ͨ�����������ռ��λ�ã�Ŀ��㣬�Լ�����������������任����
		void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);

		//��ȡ����任����
		XMMATRIX GetViewMatrix()const;

		//��ȡ��������任����
		XMMATRIX GetBaseViewMatrix()const;

		//��������任����
		void UpdateViewMatrix();

		//��������任����
		void UpdateBaseViewMatrix();


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
	};
#endif 


}
