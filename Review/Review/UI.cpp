#include "UI.h"

//D3D交换链
ComPtr<IDXGISwapChain> Control::mSwapChain;	//共享交换链

//D2D
ComPtr<ID2D1Factory> Control::md2dFactory;					//D2D工厂
ComPtr<IDWriteFactory> Control::mdwriteFactory;				//DWrite工厂
ComPtr<ID2D1RenderTarget> Control::md2dRenderTarget;			//D2D渲染目标
ComPtr<IDXGISurface> Control::mSurface;						//DXGI表面渲染目标

bool Control::InitControl(const ComPtr<IDXGISwapChain> & swapChain)
{
	mSwapChain = swapChain;//获取交换链

	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, md2dFactory.GetAddressOf()));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(mdwriteFactory.GetAddressOf())));
	return true;
}

void Control::Resize()
{
	// 为D2D创建DXGI表面渲染目标
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf());
	surface.Reset();
}
