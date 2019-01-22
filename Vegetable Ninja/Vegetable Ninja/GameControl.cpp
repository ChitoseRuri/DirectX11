#include "GameControl.h"

DX11Game::GameControl::GameControl()
{
}

DX11Game::GameControl::~GameControl()
{
}

void DX11Game::GameControl::AddSprite3D(Sprite3D * sprite, XMFLOAT3 &initialVelocity)
{
	m_Sprites.push_back(sprite);
	m_Velocity.push_back(initialVelocity);
}

void DX11Game::GameControl::DelSprite3D(unsigned num)
{
	m_Sprites.erase(m_Sprites.begin() + num);
	m_Velocity.erase(m_Velocity.begin() + num);
}

void DX11Game::GameControl::Clear()
{
	m_Sprites.clear();
	m_Velocity.clear();
}

void DX11Game::GameControl::AddGravity(XMFLOAT3 & gravity)
{
	m_Gravity.push_back(gravity);
}

void DX11Game::GameControl::UpData(float rate)
{
	int size = m_Sprites.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Sprites[i]->m_Flags == 1)
		{
			m_Sprites.erase(m_Sprites.begin() + i);
			m_Velocity.erase(m_Velocity.begin() + i);
			break;
		}
		m_Sprites[i]->Move(XMFLOAT3(m_Velocity[i].x*rate, m_Velocity[i].y*rate, m_Velocity[i].z*rate));
		XMFLOAT3 pos = m_Sprites[i]->GetTranslationRelative();
		if (pos.x > 1.0f || pos.x < -1.0f || pos.y > 0.6f || pos.y < -0.6f)
		{
			m_Sprites[i]->m_Flags = 1;
			m_Sprites.erase(m_Sprites.begin() + i);
			m_Velocity.erase(m_Velocity.begin() + i);
			i--;
			size--;
		}
	}
	for (XMFLOAT3 &velocity : m_Velocity)
	{
		for (XMFLOAT3 &gravity : m_Gravity)
		{
			velocity.x += gravity.x*rate;
			velocity.y += gravity.y*rate;
			velocity.z += gravity.z*rate;
		}
	}
}
