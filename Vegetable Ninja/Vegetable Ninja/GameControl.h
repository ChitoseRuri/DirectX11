#pragma once
#include "Sprite3D.h"

namespace DX11Game
{
	class GameControl
	{
	public:
		GameControl();
		~GameControl();
		//将一个3D精灵放入重力控制中
		void AddSprite3D(Sprite3D * vegetable, XMFLOAT3 &initialVelocity);
		//在重力控制列表移除一个精灵
		void DelSprite3D(unsigned num);
		//清空重力控制的精灵列表
		void Clear();
		void AddGravity(XMFLOAT3 &gravity);
		void UpData(float rate);

	private:
		std::vector<Sprite3D *>    m_Sprites;
		std::vector<XMFLOAT3> m_Gravity;//地域引力
		std::vector<XMFLOAT3> m_Velocity;
	};
}