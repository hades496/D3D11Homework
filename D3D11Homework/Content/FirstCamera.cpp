#include "pch.h"
#include "FirstCamera.h"

using namespace DirectX;
using namespace D3D11Homework;

FirstCamera::FirstCamera()
{
	//初始化第一人称相机的参数，刚开始相机是位于中间的
	mPosition = { 8.0f, 2.0f, 8.0f, 0.0f };
	mUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	mLook = { 0.0f, -0.1f, 0.0f, 0.0f };

	rotateY = 0.0f;
}

FirstCamera::FirstCamera(const FirstCamera&other)
{
	
}

FirstCamera::~FirstCamera()
{

}


//通过相机在世界空间的位置，目标点，以及上向量来定义相机变换矩阵
XMMATRIX FirstCamera::LookAt()
{
	XMVECTOR pos = XMLoadFloat4(&mPosition);
	XMVECTOR at = XMLoadFloat4(&mLook);
	XMVECTOR up = XMLoadFloat4(&mUp);

	return XMMatrixLookAtRH(pos, at, up);
}


//往相机坐标系Look的方向前进或者后退,然后此时Look,Up,Right都不改变,仅仅是位置在改变
void FirstCamera::Walk(float d)
{
	XMVECTOR pos = XMLoadFloat4(&mPosition);
	XMVECTOR at = XMLoadFloat4(&mLook);
	XMVECTOR up = XMLoadFloat4(&mUp);

	XMVECTOR dir = at - pos; 
	XMVector3Normalize(dir);

	XMVECTOR targetPos = pos + dir * d;
	XMVECTOR targetAt = at + dir * d;

	XMFLOAT4 temp;
	XMStoreFloat4(&temp, targetPos);
	if (temp.y < 1.0f) return;

	XMStoreFloat4(&mPosition, targetPos);
	XMStoreFloat4(&mLook, targetAt);
}

//往相机坐标系Right方向前进或者后退,然后此时Look,Up,Right都不改变,仅仅是位置在改变
void FirstCamera::Strafe(float d)
{
	XMVECTOR pos = XMLoadFloat4(&mPosition);
	XMVECTOR at = XMLoadFloat4(&mLook);
	XMVECTOR up = XMLoadFloat4(&mUp);

	XMVECTOR dir = XMVector3Cross(up, at - pos);
	XMVector3Normalize(dir);

	XMVECTOR targetPos = pos + dir * d;
	XMVECTOR targetAt = at + dir * d;

	XMFLOAT4 temp;
	XMStoreFloat4(&temp, targetPos);
	if (temp.y < 1.0f) return;

	XMStoreFloat4(&mPosition, targetPos);
	XMStoreFloat4(&mLook, targetAt);
}

//相机上下的移动是位置在Y值上的改变，而Look,Up,Right都不改变,仅仅是位置在改变
void FirstCamera::UpDown(float d)
{
	XMVECTOR pos = XMLoadFloat4(&mPosition);
	XMVECTOR at = XMLoadFloat4(&mLook);
	XMVECTOR up = XMLoadFloat4(&mUp);

	XMVECTOR dir = up;
	XMVector3Normalize(dir);

	XMVECTOR targetPos = pos + dir * d * 4;
	XMVECTOR targetAt = at + dir * d * 4;
	
	XMFLOAT4 temp;
	XMStoreFloat4(&temp, targetPos);
	if (temp.y < 1.0f) return;

	XMStoreFloat4(&mPosition, targetPos);
	XMStoreFloat4(&mLook, targetAt);
}


//Look和Up向量绕相机坐标系的right向量进行旋转，此时look和up向量改变，而right和poisiton不改变
void FirstCamera::Pitch(float angle)
{
	XMVECTOR pos = XMLoadFloat4(&mPosition);
	XMVECTOR at = XMLoadFloat4(&mLook);
	XMVECTOR up = XMLoadFloat4(&mUp);

	XMVECTOR dir = up;
	XMVector3Normalize(dir);

	XMVECTOR targetPos = pos + dir * angle;
	XMVECTOR targetAt = at + dir * angle;

	XMStoreFloat4(&mPosition, targetPos);
	XMStoreFloat4(&mLook, targetAt);
}

//绕世界坐标系的Y轴进行旋转(而非相机坐标系的Up向量,这点容易受到惯性思维而误解，实在无法理解就用人的视角在现实世界是怎么旋转的粒子解释),look,up,right绕Y轴进行旋转，位置不变
void FirstCamera::RotateY(float angle)
{
	
}


void FirstCamera::Update()
{
	float movePerFrame = m_movePerSecond / 60;
	if (Listener.IsWPressed())
	{
		Walk(movePerFrame);
	}
	else if (Listener.IsSPressed())
	{
		Walk(-movePerFrame);
	}
	
	if (Listener.IsAPressed())
	{
		Strafe(movePerFrame);
	}
	else if (Listener.IsDPressed())
	{
		Strafe(-movePerFrame);
	}

	if (Listener.IsQPressed())
	{
		UpDown(movePerFrame);
	}
	else if (Listener.IsEPressed())
	{
		UpDown(-movePerFrame);
	}

	
	//Listener.GetMousePositionOffset(mouseXOffset, mouseYOffset);

	////进行视角上下的旋转(跟刚开始的旋转角度在正负90度之间)  
	//if (rotateY <= 90.0f&&rotateY >= -90.0f)
	//{
	//	rotateY += (float)mouseYOffset*0.007f*DeltaTime*(float)FPS / 4000.0f;
	//	mFirstCameraClass->Pitch((float)mouseYOffset*0.007f*DeltaTime*(float)FPS / 4000.0f);
	//}

	////进行视角左右的旋转  
	//mFirstCameraClass->RotateY((float)mouseXOffset*0.007f*DeltaTime*(float)FPS / 4000.0f);


}