#include "Camera.h"

Camera::Camera()
{
	m_Position = XMFLOAT3(0.0f,0.0f,0.0f);
	m_Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_Target = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Front = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_View = XMMatrixIdentity();
	m_Proj = XMMatrixIdentity();
}

Camera::~Camera()
{
}

void Camera::SetPosition(XMFLOAT3 position)
{
	m_Position = position;
	CalculateRight();
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	CalculateRight();
}

void Camera::SetTarget(XMFLOAT3 target)
{
	m_Target = target;
	CalculateRight();
}

void Camera::SetTarget(float x, float y, float z)
{
	m_Target.x = x;
	m_Target.y = y;
	m_Target.z = z;
	CalculateRight();
}

void Camera::SetUpNormal(XMFLOAT3 upNormal)
{
	m_Up = upNormal;
	CalculateRight();
}

void Camera::SetUpNormal(float x, float y, float z)
{
	m_Up.x = x;
	m_Up.y = y;
	m_Up.z = z;
	CalculateRight();
}

void Camera::CalculateRight()
{
	XMVECTOR up = XMLoadFloat3(&m_Up);
	XMVECTOR front = XMLoadFloat3(&m_Front);
	XMStoreFloat3(&m_Right, XMVector3Normalize(XMVector3Cross(up, front)));
}

XMFLOAT3 Camera::GetPosition()
{
	return m_Position;
}

XMFLOAT3 Camera::GetTarget()
{
	return m_Target;
}

XMFLOAT3 Camera::GetUpNormal()
{
	return m_Up;
}

void Camera::MoveAD(float length)
{
	XMVECTOR moveLength = XMLoadFloat3(&m_Right);
	moveLength *= length;
	XMFLOAT3 addLength;
	XMStoreFloat3(&addLength, moveLength);
	m_Position.x += addLength.x;
	m_Position.y += addLength.y;
	m_Position.z += addLength.z;
	m_Target.x += addLength.x;
	m_Target.y += addLength.y;
	m_Target.z += addLength.z;
}

void Camera::MoveWS(float length)
{
	XMVECTOR front = XMLoadFloat3(&m_Front);
	XMVectorSetY(front, 0.0f);
	front = XMVector3Normalize(front);
	front *= length;
	XMFLOAT3 addLength;
	XMStoreFloat3(&addLength, front);
	m_Position.x += addLength.x;
	m_Position.z += addLength.z;
	m_Target.x += addLength.x;
	m_Target.z += addLength.z;
}

void Camera::MoveUpDown(float length)
{
	m_Position.x += m_Up.x*length;
	m_Position.y += m_Up.y*length;
	m_Position.z += m_Up.z*length;
	m_Target.x += m_Up.x*length;
	m_Target.y += m_Up.y*length;
	m_Target.z += m_Up.z*length;
}

void Camera::Rotate(float radian, XMFLOAT3 rota)
{
	FXMVECTOR roVector = XMLoadFloat3(&rota);
	XMMATRIX tranMatrix =  XMMatrixRotationAxis(roVector, radian);
	XMVECTOR rightVector = XMLoadFloat3(&m_Right);
	XMVECTOR frontVector = XMLoadFloat3(&m_Front);
	rightVector = XMVector3Transform(rightVector, tranMatrix);
	frontVector = XMVector3Transform(frontVector, tranMatrix);
	rightVector = XMVector3Normalize(rightVector);
	frontVector = XMVector3Normalize(frontVector);
	XMStoreFloat3(&m_Right, rightVector);
	XMStoreFloat3(&m_Front, frontVector);
	m_Target.x = m_Position.x + m_Front.x;
	m_Target.y = m_Position.y + m_Front.y;
	m_Target.z = m_Position.z + m_Front.z;
}

void Camera::Rotate(float radian)
{
	Rotate(radian, m_Up);
}

void Camera::Raise(float radian, XMFLOAT3 rota)
{
	FXMVECTOR roVector = XMLoadFloat3(&rota);
	XMMATRIX tranMatrix = XMMatrixRotationAxis(roVector, radian);
	XMVECTOR front = XMLoadFloat3(&m_Front);
	front = XMVector3Transform(front, tranMatrix);
	front = XMVector3Normalize(front);
	if (XMVectorGetY(front) > 0.9f)
	{
		front = XMVectorSetY(front,0.9f);
		front = XMVector3Normalize(front);
	}
	else if (XMVectorGetY(front) < -0.9f)
	{
		front = XMVectorSetY(front,-0.9f);
		front = XMVector3Normalize(front);
	}
	XMStoreFloat3(&m_Front, front);
	m_Target.x = m_Position.x + m_Front.x;
	m_Target.y = m_Position.y + m_Front.y;
	m_Target.z = m_Position.z + m_Front.z;
}

void Camera::Raise(float radian)
{
	Raise(radian, m_Right);
}

void Camera::RotateWith(float radian, XMFLOAT3 rota, XMFLOAT3 point)
{
	XMVECTOR rotaV = XMLoadFloat3(&rota);
	XMVECTOR front = XMLoadFloat3(&m_Front);
	XMVECTOR up = XMLoadFloat3(&m_Up);
	XMVECTOR bothPosition = XMLoadFloat3(&m_Position) - XMLoadFloat3(&point);	//获得坐标与点的向量
	XMVECTOR bothTarget = XMLoadFloat3(&m_Target) - XMLoadFloat3(&point);			//目标与点的向量
	XMMATRIX tranMatrix = XMMatrixRotationAxis(rotaV, radian);							
	bothPosition = XMVector3Transform(bothPosition, tranMatrix);								//两点的向量围着目标向量旋转
	bothTarget = XMVector3Transform(bothTarget, tranMatrix);	
	up = XMVector3Transform(up, tranMatrix);																	//上向量旋转
	XMFLOAT3 lengthNormal;																									//目标点加上选择后的向量得出摄像机位置
	XMStoreFloat3(&lengthNormal, bothPosition);
	m_Position.x = point.x + lengthNormal.x;
	m_Position.y = point.y + lengthNormal.y;
	m_Position.z = point.z + lengthNormal.z;
	XMStoreFloat3(&lengthNormal, bothTarget);
	m_Target.x = point.x + lengthNormal.x;
	m_Target.y = point.y + lengthNormal.y;
	m_Target.z = point.z + lengthNormal.z;
	XMStoreFloat3(&m_Front, XMVector3Normalize(bothTarget - bothPosition));	//重置前向量																					//重置前向量
	CalculateRight();	
}

void Camera::SetPerspectiveMatrix(float radian, float scale, float nearView, float farView)
{
	m_Proj = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, scale, nearView, farView);
}

XMMATRIX Camera::GetPerspectiveMatrix()
{
	return m_Proj;
}

XMMATRIX Camera::GetViewMatrix()
{
	m_View = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
	return m_View;
}