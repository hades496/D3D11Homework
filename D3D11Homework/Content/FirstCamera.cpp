#include "pch.h"
#include "FirstCamera.h"

using namespace DirectX;
using namespace D3D11Homework;

FirstCamera::FirstCamera()
{
	//��ʼ����һ�˳�����Ĳ������տ�ʼ�����λ���м��
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


//ͨ�����������ռ��λ�ã�Ŀ��㣬�Լ�����������������任����
XMMATRIX FirstCamera::LookAt()
{
	XMVECTOR pos = XMLoadFloat4(&mPosition);
	XMVECTOR at = XMLoadFloat4(&mLook);
	XMVECTOR up = XMLoadFloat4(&mUp);

	return XMMatrixLookAtRH(pos, at, up);
}


//���������ϵLook�ķ���ǰ�����ߺ���,Ȼ���ʱLook,Up,Right�����ı�,������λ���ڸı�
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

//���������ϵRight����ǰ�����ߺ���,Ȼ���ʱLook,Up,Right�����ı�,������λ���ڸı�
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

//������µ��ƶ���λ����Yֵ�ϵĸı䣬��Look,Up,Right�����ı�,������λ���ڸı�
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


//Look��Up�������������ϵ��right����������ת����ʱlook��up�����ı䣬��right��poisiton���ı�
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

//����������ϵ��Y�������ת(�����������ϵ��Up����,��������ܵ�����˼ά����⣬ʵ���޷��������˵��ӽ�����ʵ��������ô��ת�����ӽ���),look,up,right��Y�������ת��λ�ò���
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

	////�����ӽ����µ���ת(���տ�ʼ����ת�Ƕ�������90��֮��)  
	//if (rotateY <= 90.0f&&rotateY >= -90.0f)
	//{
	//	rotateY += (float)mouseYOffset*0.007f*DeltaTime*(float)FPS / 4000.0f;
	//	mFirstCameraClass->Pitch((float)mouseYOffset*0.007f*DeltaTime*(float)FPS / 4000.0f);
	//}

	////�����ӽ����ҵ���ת  
	//mFirstCameraClass->RotateY((float)mouseXOffset*0.007f*DeltaTime*(float)FPS / 4000.0f);


}