#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

bool UI::InitD2d(const ComPtr<IDXGISwapChain>& swapChain)
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
	md2dRenderTarget.Reset();
	Label::BeforeResize();
}

void UI::AfterD3dResize()
{
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf());
	surface.Reset();
	if (hr == E_NOINTERFACE)
	{
		OutputDebugString(L"\n警告：Direct2D与Direct3D互操作性功能受限，你将无法看到文本信息。现提供下述可选方法：\n"
			"1. 对于Win7系统，需要更新至Win7 SP1，并安装KB2670838补丁以支持Direct2D显示。\n"
			"2. 自行完成Direct3D 10.1与Direct2D的交互。详情参阅："
			"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
			"3. 使用别的字体库，比如FreeType。\n\n");
	}
	else if (hr == S_OK)
	{
		Control::InitControl(mdwriteFactory, md2dRenderTarget);
		Label::AfterResize();	
	}
	else
	{
		// 报告异常问题
		assert(md2dRenderTarget);
	}
}

void UI::DrawScene()
{
	md2dRenderTarget->BeginDraw();
	for (auto & var : mLabels)
	{
		var.DrawScene();
	}
	HR(md2dRenderTarget->EndDraw());
}
