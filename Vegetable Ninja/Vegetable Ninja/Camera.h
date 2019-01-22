#ifndef CAMERA_H
#define CAMERA_H
#include "d3dUtil.h"

class Camera
{
public:
	Camera();
	~Camera();
	//���������λ��
	void SetPosition(XMFLOAT3 position);				
	//���������λ��
	void SetPosition(float x, float y, float z);
	//���������Ŀ��
	void SetTarget(XMFLOAT3 target);						
	//���������Ŀ��
	void SetTarget(float x, float y, float z);
	//���������������
	void SetUpNormal(XMFLOAT3 upNormal);		
	//���������������
	void SetUpNormal(float x, float y, float z);
	//����������
	void CalculateRight();												
	//��������λ��
	XMFLOAT3 GetPosition();										
	//��������Ŀ��
	XMFLOAT3 GetTarget();											
	//��������������
	XMFLOAT3 GetUpNormal();									
	//����������ƶ�
	void MoveAD(float length);										
	//�����ǰ���ƶ�
	void MoveWS(float length);										
	//����������ƶ�
	void MoveUpDown(float length);							
	//������ת
	void Rotate(float radian,XMFLOAT3 rota);		
	//������ת
	void Rotate(float radian);
	//������ת
	void Raise(float radian,XMFLOAT3 rota);
	//������ת
	void Raise(float radian);
	//������Ƶ���ת
	void RotateWith(float radian, XMFLOAT3 rota, XMFLOAT3 point);
	//����͸�Ӿ���
	void SetPerspectiveMatrix(float radian, float scale, float nearView, float farView); 
	//���͸�Ӿ���
	XMMATRIX GetPerspectiveMatrix();	
	//����Ӿ���
	XMMATRIX GetViewMatrix();								

private:
	XMFLOAT3 m_Up;													//������
	XMFLOAT3 m_Target;												//Ŀ���
	XMFLOAT3 m_Position;											//�����λ��
	XMFLOAT3 m_Front;												//ǰ����
	XMFLOAT3 m_Right;												//������
	XMMATRIX m_View;												//�Ӿ���
	XMMATRIX m_Proj;													//͸�Ӿ���
};
#endif // !CAMERA_H
