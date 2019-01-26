#pragma once
#include <wrl/client.h>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include "DXTrace.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


// 使用模板别名(C++11)简化类型名

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class Control
{
protected:

	//D3D交换链
	static ComPtr<IDXGISwapChain> mSwapChain;	//共享交换链

	//D2D
	static ComPtr<ID2D1Factory> md2dFactory;					//D2D工厂
	static ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite工厂
	static ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D渲染目标
	static ComPtr<IDXGISurface> mSurface;						//DXGI表面渲染目标

public:
	static bool InitControl(const ComPtr<IDXGISwapChain> & swapChain);//初始化控件系统
	static void Resize();											  //在调用子类的Resize前调用
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