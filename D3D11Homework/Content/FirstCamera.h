#pragma once

#ifndef  _FIRST_CAMERA_CLASS_H
#define _FIRST_CAMERA_CLASS_H
//这个类封装了相机的位置和旋转量
#include<Windows.h>

using namespace DirectX;
namespace D3D11Homework
{
	class FirstCamera
	{
	private:
		//相机在世界空间的位置和方向向量
		XMFLOAT3 mPosition;
		XMFLOAT3 mRight;
		XMFLOAT3 mUp;
		XMFLOAT3 mLook;

		//缓存相机变换矩阵
		XMMATRIX mViewMatrix;
		XMMATRIX mBaseViewMatrix;

	public:
		FirstCamera();
		FirstCamera(const FirstCamera& other);
		~FirstCamera();

	public:
		//相机位置
		void SetPosition(float x, float y, float z);
		void SetPosition(const XMFLOAT3& v);
		XMFLOAT3 GetPosition()const;
		XMVECTOR GetPositionXM()const;

		//获取相机的基础向量(Up,Look,Right)
		XMFLOAT3 GetUp()const;
		XMVECTOR GetUpXM()const;
		XMFLOAT3 GetLook()const;
		XMVECTOR GetLookXM()const;
		XMFLOAT3 GetRight()const;
		XMVECTOR GetRightXM()const;

		//通过相机在世界空间的位置，目标点，以及上向量来定义相机变换矩阵
		void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);

		//获取相机变换矩阵
		XMMATRIX GetViewMatrix()const;

		//获取基础相机变换矩阵
		XMMATRIX GetBaseViewMatrix()const;

		//更新相机变换矩阵
		void UpdateViewMatrix();

		//更新相机变换矩阵
		void UpdateBaseViewMatrix();


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
	};
#endif 


}
