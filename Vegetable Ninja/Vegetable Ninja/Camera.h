#ifndef CAMERA_H
#define CAMERA_H
#include "d3dUtil.h"

class Camera
{
public:
	Camera();
	~Camera();
	//设置摄像机位置
	void SetPosition(XMFLOAT3 position);				
	//设置摄像机位置
	void SetPosition(float x, float y, float z);
	//设置摄像机目标
	void SetTarget(XMFLOAT3 target);						
	//设置摄像机目标
	void SetTarget(float x, float y, float z);
	//设置摄像机上向量
	void SetUpNormal(XMFLOAT3 upNormal);		
	//设置摄像机上向量
	void SetUpNormal(float x, float y, float z);
	//计算右向量
	void CalculateRight();												
	//获得摄像机位置
	XMFLOAT3 GetPosition();										
	//获得摄像机目标
	XMFLOAT3 GetTarget();											
	//获得摄像机上向量
	XMFLOAT3 GetUpNormal();									
	//摄像机左右移动
	void MoveAD(float length);										
	//摄像机前后移动
	void MoveWS(float length);										
	//摄像机上下移动
	void MoveUpDown(float length);							
	//左右旋转
	void Rotate(float radian,XMFLOAT3 rota);		
	//左右旋转
	void Rotate(float radian);
	//上下旋转
	void Raise(float radian,XMFLOAT3 rota);
	//上下旋转
	void Raise(float radian);
	//摄像机绕点旋转
	void RotateWith(float radian, XMFLOAT3 rota, XMFLOAT3 point);
	//设置透视矩阵
	void SetPerspectiveMatrix(float radian, float scale, float nearView, float farView); 
	//获得透视矩阵
	XMMATRIX GetPerspectiveMatrix();	
	//获得视矩阵
	XMMATRIX GetViewMatrix();								

private:
	XMFLOAT3 m_Up;													//上向量
	XMFLOAT3 m_Target;												//目标点
	XMFLOAT3 m_Position;											//摄像机位置
	XMFLOAT3 m_Front;												//前向量
	XMFLOAT3 m_Right;												//右向量
	XMMATRIX m_View;												//视矩阵
	XMMATRIX m_Proj;													//透视矩阵
};
#endif // !CAMERA_H
