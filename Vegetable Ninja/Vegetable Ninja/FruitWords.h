#pragma once
#include "Sprite3D.h"
#include "DirectWrite.h"

namespace DX11Game
{
	wchar_t * CharToWchar(LPCSTR pszSrc, int nLen);
	//����ˮ���Լ���������Ϣ�Ľṹ��
	struct FruitPairs
	{
		//3D����
		Sprite3D Fruit;
		//ˮ������
		int Type;
		//�����±�
		int QASub;
		//2D����
		wchar_t * Wchar;
		//D2dʵ��
		DirectWrite Words;
		FruitPairs() {}
		FruitPairs(int & type, int & sub, wchar_t * words) :Type(type), QASub(sub), Wchar(words) {}
		FruitPairs(int & type, int & sub, wchar_t * words, IDXGISwapChain * swapChain, HWND hwnd) :Type(type), QASub(sub), Wchar(words)
		{
			Words.Initialize(swapChain, hwnd);
			Words.SetFont(L"΢���ź�", 24.0f);
			Words.SetParagraphAlignment();
			Words.SetTextAlignment();
		}
	};
}