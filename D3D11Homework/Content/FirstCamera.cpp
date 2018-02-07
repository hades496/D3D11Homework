#include "pch.h"
#include "FirstCamera.h"

using namespace DirectX;
using namespace D3D11Homework;

FirstCamera::FirstCamera()
{
	//��ʼ����һ�˳�����Ĳ������տ�ʼ�����λ���м��
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

//���λ��
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



//��ȡ����Ļ�������(Up,Look,Right)
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

//��ȡ����任����
XMMATRIX FirstCamera::GetViewMatrix()const
{
	return mViewMatrix;
}

//ͨ�����������ռ��λ�ã�Ŀ��㣬�Լ�����������������任����
void FirstCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{

	XMVECTOR Look = XMVectorSubtract(target, pos);
	XMVECTOR Up = worldUp;
	XMVECTOR Right = XMVector3Cross(Up, Look);
	Up = XMVector3Cross(Look, Right);

	//�����������
	XMVector3Normalize(Look);
	XMVector3Normalize(Up);
	XMVector3Normalize(Right);

	//����������Camera����
	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, Look);
	XMStoreFloat3(&mUp, Up);
	XMStoreFloat3(&mRight, Right);

}


//���������ϵLook�ķ���ǰ�����ߺ���,Ȼ���ʱLook,Up,Right�����ı�,������λ���ڸı�
void FirstCamera::Walk(float d)
{
	//mPosition+=d*mLook
	//XMVectorReplicate���ص���XMVECTOR(d,d,d,d)
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

//���������ϵRight����ǰ�����ߺ���,Ȼ���ʱLook,Up,Right�����ı�,������λ���ڸı�
void FirstCamera::Strafe(float d)
{
	//mPosition+=d*mRight
	//XMVectorReplicate���ص���XMVECTOR(d,d,d,d)
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

//������µ��ƶ���λ����Yֵ�ϵĸı䣬��Look,Up,Right�����ı�,������λ���ڸı�
void FirstCamera::UpDown(float d)
{

	//mPosition+=d*up
	//XMVectorReplicate���ص���XMVECTOR(d,d,d,d)
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR p = XMLoadFloat3(&mPosition);

	//XMVectorMultiplyAdd(v1,v2,v3)=v1*v2+v3
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, up, p));
}


//Look��Up�������������ϵ��right����������ת����ʱlook��up�����ı䣬��right��poisiton���ı�
void FirstCamera::Pitch(float angle)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
}

//����������ϵ��Y�������ת(�����������ϵ��Up����,��������ܵ�����˼ά����⣬ʵ���޷��������˵��ӽ�����ʵ��������ô��ת�����ӽ���),look,up,right��Y�������ת��λ�ò���
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

	//�������right,look,up

	//���look����
	L = XMVector3Normalize(L);

	//U=look(X)right
	U = XMVector3Normalize(XMVector3Cross(L, R));

	//R=up(X)look
	R = XMVector3Normalize(XMVector3Cross(U, L));

	//�������任����ĳЩ��������ĵ��
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

//��������任����
void FirstCamera::UpdateBaseViewMatrix()
{
	//������,λ������,�۲�����
	XMVECTOR Up, Postion, LookAt;

	//���������λ��
	Postion = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	LookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//��󴴽�ViewMatrix
	mBaseViewMatrix = XMMatrixLookAtLH(Postion, LookAt, Up);
}