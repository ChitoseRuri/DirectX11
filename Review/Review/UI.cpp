#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

bool UI::InitControl(const ComPtr<IDXGISwapChain>& swapChain)
{
	//获取交换链的共享
	mSwapChain = swapChain;
	//创建D2D工厂
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, md2dFactory.GetAddressOf()));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(mdwriteFactory.GetAddressOf())));
	//初始化控件系统
	Control::InitControl(mdwriteFactory, md2dRenderTarget);

	return true;
}

void UI::BeforeD3dResize()
{
	Label::BeforeResize();
}

void UI::AfterD3dResize()
{
	Label::AfterResize(md2dFactory,mSwapChain);
}
