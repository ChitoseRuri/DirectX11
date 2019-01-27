#pragma once
#include "Label.h"




class UI
{
private:
	//D3D交换链
	ComPtr<IDXGISwapChain> mSwapChain;	//共享交换链
	//D2D
	ComPtr<ID2D1Factory> md2dFactory;					//D2D工厂
	ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite工厂
	ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D渲染目标
	ComPtr<IDXGISurface> mSurface;						//DXGI表面渲染目标

	std::vector<Label> mLabels;							

public:
	UI();
	~UI();
	bool InitControl(const ComPtr<IDXGISwapChain> & swapChain);//初始化控件系统
	void BeforeD3dResize();
	void AfterD3dResize();
	void UpdateScene(float dt);
	void DrawScene();

};