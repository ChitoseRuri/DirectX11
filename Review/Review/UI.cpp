#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

bool UI::InitControl(const ComPtr<IDXGISwapChain>& swapChain)
{
	//��ȡ�������Ĺ���
	mSwapChain = swapChain;
	//����D2D����
	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, md2dFactory.GetAddressOf()));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(mdwriteFactory.GetAddressOf())));
	//��ʼ���ؼ�ϵͳ
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
