#pragma once
#include "Sprite3D.h"

namespace DX11Game
{
	class GameControl
	{
	public:
		GameControl();
		~GameControl();
		//��һ��3D�����������������
		void AddSprite3D(Sprite3D * vegetable, XMFLOAT3 &initialVelocity);
		//�����������б��Ƴ�һ������
		void DelSprite3D(unsigned num);
		//����������Ƶľ����б�
		void Clear();
		void AddGravity(XMFLOAT3 &gravity);
		void UpData(float rate);

	private:
		std::vector<Sprite3D *>    m_Sprites;
		std::vector<XMFLOAT3> m_Gravity;//��������
		std::vector<XMFLOAT3> m_Velocity;
	};
}