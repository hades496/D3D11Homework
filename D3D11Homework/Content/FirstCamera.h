#pragma once

#include<InputClass.h>
using namespace DirectX;
namespace D3D11Homework
{
	class FirstCamera
	{
	private:

		//相机在世界空间的位置和方向向量
		XMFLOAT4 mPosition;
		XMFLOAT4 mUp;
		XMFLOAT4 mLook;
		int mouseXOffset, mouseYOffset;
		float rotateY;

		//监听用户操作
		InputClass Listener;
		float m_movePerSecond = 0.5f;

	public:
		FirstCamera();
		FirstCamera(const FirstCamera& other);
		~FirstCamera();

	public:
		//通过相机在世界空间的位置，目标点，以及上向量来定义相机变换矩阵
		XMMATRIX LookAt();

		//左右移动(沿着右向量移动)
		void Strafe(float d);

		//前后移动(沿着LookAt向量移动)
		void Walk(float d);

		//上下移动(沿着世界空间的Y轴移动)
		void UpDown(float d);

		//绕相机的右向量旋转
		void Pitch(float angle);

		//绕世界空间的Y轴进行旋转,注意旋转角度应该在初始方向的正负90度之间,虚幻四引擎就是这样的(-90.0f<=RotateAngle<=90.0f)
		void RotateY(float angle);

		//每帧更新相机位置
		void Update();
	};
}