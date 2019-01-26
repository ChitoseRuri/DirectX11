#pragma once
#include <wrl/client.h>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include "DXTrace.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


// ʹ��ģ�����(C++11)��������

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Control
{
protected:

	//D3D������
	static ComPtr<IDXGISwapChain> mSwapChain;	//��������

	//D2D
	static ComPtr<ID2D1Factory> md2dFactory;					//D2D����
	static ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite����
	static ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D��ȾĿ��
	static ComPtr<IDXGISurface> mSurface;						//DXGI������ȾĿ��

public:
	static bool InitControl(const ComPtr<IDXGISwapChain> & swapChain);//��ʼ���ؼ�ϵͳ
	static void Resize();											  //�ڵ��������Resizeǰ����
	virtual void OnResize() = 0;
	virtual void DrawScene() = 0;
	virtual void UpdateScene(float dt) = 0;

private:
	Control();
	~Control();
};

class Label :public Control
{
private:
	
public:
	void OnResize() override;
	void DrawScene() override;
	void UpdateScene(float dt) override;
};