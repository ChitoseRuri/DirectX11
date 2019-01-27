#pragma once
#include "Label.h"




class UI
{
private:
	//D3D������
	ComPtr<IDXGISwapChain> mSwapChain;	//��������
	//D2D
	ComPtr<ID2D1Factory> md2dFactory;					//D2D����
	ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite����
	ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D��ȾĿ��
	ComPtr<IDXGISurface> mSurface;						//DXGI������ȾĿ��

	std::vector<Label> mLabels;							

public:
	UI();
	~UI();
	bool InitControl(const ComPtr<IDXGISwapChain> & swapChain);//��ʼ���ؼ�ϵͳ
	void BeforeD3dResize();
	void AfterD3dResize();
	void UpdateScene(float dt);
	void DrawScene();

};