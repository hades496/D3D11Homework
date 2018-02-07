#include "pch.h"
#include "FirstCamera.h"

using namespace DirectX;
using namespace D3D11Homework;

FirstCamera::FirstCamera()
{
	//初始化第一人称相机的参数，刚开始相机是位于中间的
	mPosition = XMFLOAT3(0.f, 0.0f, -5.0f);
	mRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	mUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	mLook = XMFLOAT3(0.0f, 0.0f, 1.0f);
}

FirstCamera::FirstCamera(const FirstCamera&other)
{

}

FirstCamera::~FirstCamera()
{

}

//相机位置
void FirstCamera::SetPosition(float x, float y, float z)
{
	mPosition.x = x;
	mPosition.y = y;
	mPosition.z = z;
}


void FirstCamera::SetPosition(const XMFLOAT3& v)
{
	mPosition = v;
}


XMFLOAT3 FirstCamera::GetPosition()const
{
	return mPosition;
}

XMVECTOR FirstCamera::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}



//获取相机的基础向量(Up,Look,Right)
XMFLOAT3 FirstCamera::GetUp()const
{
	return mUp;
}
XMVECTOR FirstCamera::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}


XMFLOAT3 FirstCamera::GetLook()const
{
	return mLook;
}
XMVECTOR FirstCamera::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}


XMFLOAT3 FirstCamera::GetRight()const
{

	return mRight;
}

XMVECTOR FirstCamera::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

//获取相机变换矩阵
XMMATRIX FirstCamera::GetViewMatrix()const
{
	return mViewMatrix;
}

//通过相机在世界空间的位置，目标点，以及上向量来定义相机变换矩阵
void FirstCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{

	XMVECTOR Look = XMVectorSubtract(target, pos);
	XMVECTOR Up = worldUp;
	XMVECTOR Right = XMVector3Cross(Up, Look);
	Up = XMVector3Cross(Look, Right);

	//规格化三个先来
	XMVector3Normalize(Look);
	XMVector3Normalize(Up);
	XMVector3Normalize(Right);

	//将参数存入Camera类中
	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, Look);
	XMStoreFloat3(&mUp, Up);
	XMStoreFloat3(&mRight, Right);

}


//往相机坐标系Look的方向前进或者后退,然后此时Look,Up,Right都不改变,仅仅是位置在改变
void FirstCamera::Walk(float d)
{
	//mPosition+=d*mLook
	//XMVectorReplicate返回的是XMVECTOR(d,d,d,d)
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

//往相机坐标系Right方向前进或者后退,然后此时Look,Up,Right都不改变,仅仅是位置在改变
void FirstCamera::Strafe(float d)
{
	//mPosition+=d*mRight
	//XMVectorReplicate返回的是XMVECTOR(d,d,d,d)
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

//相机上下的移动是位置在Y值上的改变，而Look,Up,Right都不改变,仅仅是位置在改变
void FirstCamera::UpDown(float d)
{

	//mPosition+=d*up
	//XMVectorReplicate返回的是XMVECTOR(d,d,d,d)
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, up, p));
}


//Look和Up向量绕相机坐标系的right向量进行旋转，此时look和up向量改变，而right和poisiton不改变
void FirstCamera::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
}

//绕世界坐标系的Y轴进行旋转(而非相机坐标系的Up向量,这点容易受到惯性思维而误解，实在无法理解就用人的视角在现实世界是怎么旋转的粒子解释),look,up,right绕Y轴进行旋转，位置不变
void FirstCamera::RotateY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
}

void FirstCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	//正交规格化right,look,up

	//规格化look向量
	L = XMVector3Normalize(L);

	//U=look(X)right
	U = XMVector3Normalize(XMVector3Cross(L, R));

	//R=up(X)look
	R = XMVector3Normalize(XMVector3Cross(U, L));

	//求出相机变换矩阵某些参数里面的点积
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));


	mViewMatrix = XMMATRIX(
		mRight.x,	mRight.y,	 mRight.z,	x,
		mUp.x,		mUp.y,		mUp.z,		y,
		mLook.x,	mLook.y,	mLook.z,	z,
		0,			0,			0,			1
	);
}


XMMATRIX FirstCamera::GetBaseViewMatrix()const
{
	return mBaseViewMatrix;
}

//更新相机变换矩阵
void FirstCamera::UpdateBaseViewMatrix()
{
	//上向量,位置向量,观察向量
	XMVECTOR Up, Postion, LookAt;

	//设置相机的位置
	Postion = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	LookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//最后创建ViewMatrix
	mBaseViewMatrix = XMMatrixLookAtLH(Postion, LookAt, Up);
}