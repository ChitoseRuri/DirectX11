#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

bool UI::InitD2d(const ComPtr<IDXGISwapChain>& swapChain)
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
		OutputDebugString(L"\n���棺Direct2D��Direct3D�������Թ������ޣ��㽫�޷������ı���Ϣ�����ṩ������ѡ������\n"
			"1. ����Win7ϵͳ����Ҫ������Win7 SP1������װKB2670838������֧��Direct2D��ʾ��\n"
			"2. �������Direct3D 10.1��Direct2D�Ľ�����������ģ�"
			"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
			"3. ʹ�ñ������⣬����FreeType��\n\n");
	}
	else if (hr == S_OK)
	{
		Control::InitControl(mdwriteFactory, md2dRenderTarget);
		Label::AfterResize();	
	}
	else
	{
		// �����쳣����
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
