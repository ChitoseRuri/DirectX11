#pragma once

#include "d3dUtil.h"
#include <vector>

namespace DX11Game
{
	class Anime
	{
	public:
		Anime();
		~Anime();
		void LoadPicture(char * path);

	private:
		std::vector<ID3D11ShaderResourceView*> m_PShaderResources;
		std::vector<int> m_Order;
		XMFLOAT2 m_LeftUp;
		XMFLOAT2 m_RightDown;
	};
}