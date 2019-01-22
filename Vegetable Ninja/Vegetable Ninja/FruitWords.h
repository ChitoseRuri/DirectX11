#pragma once
#include "Sprite3D.h"
#include "DirectWrite.h"

namespace DX11Game
{
	wchar_t * CharToWchar(LPCSTR pszSrc, int nLen);
	//储存水果以及其文字信息的结构体
	struct FruitPairs
	{
		//3D精灵
		Sprite3D Fruit;
		//水果类型
		int Type;
		//字体下标
		int QASub;
		//2D字体
		wchar_t * Wchar;
		//D2d实例
		DirectWrite Words;
		FruitPairs() {}
		FruitPairs(int & type, int & sub, wchar_t * words) :Type(type), QASub(sub), Wchar(words) {}
		FruitPairs(int & type, int & sub, wchar_t * words, IDXGISwapChain * swapChain, HWND hwnd) :Type(type), QASub(sub), Wchar(words)
		{
			Words.Initialize(swapChain, hwnd);
			Words.SetFont(L"微软雅黑", 24.0f);
			Words.SetParagraphAlignment();
			Words.SetTextAlignment();
		}
	};
}