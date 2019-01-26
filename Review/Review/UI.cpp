#include "UI.h"

//D3D������
ComPtr<IDXGISwapChain> Control::mSwapChain;	//��������

//D2D
ComPtr<ID2D1Factory> Control::md2dFactory;					//D2D����
ComPtr<IDWriteFactory> Control::mdwriteFactory;				//DWrite����
ComPtr<ID2D1RenderTarget> Control::md2dRenderTarget;			//D2D��ȾĿ��
ComPtr<IDXGISurface> Control::mSurface;						//DXGI������ȾĿ��

bool Control::InitControl(const ComPtr<IDXGISwapChain> & swapChain)
{
	mSwapChain = swapChain;//��ȡ������

	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, md2dFactory.GetAddressOf()));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown **>(mdwriteFactory.GetAddressOf())));
	return true;
}

void Control::Resize()
{
	// ΪD2D����DXGI������ȾĿ��
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf());
	surface.Reset();
}
