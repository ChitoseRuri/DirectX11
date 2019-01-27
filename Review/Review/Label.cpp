#include "Label.h"

using namespace DirectX;
using std::vector;
vector<ComPtr<ID2D1SolidColorBrush>> Label::mColorBrushs;				//��ɫ��ˢ
vector<D2D1::ColorF::Enum> Label::mColors;								//������ɫ����
vector<ComPtr<IDWriteTextFormat>> Label::mTextFormats;					//�ı���ʽ
vector<ComPtr<IDWriteFont>> Label::mFonts;								//����
vector<Label::Format> Label::mFormats;									//������������

Label::Label()
{
}

Label::~Label()
{
}

void Label::OnResize()
{
}

void Label::DrawScene()
{
}

void Label::UpdateScene(float dt)
{
}

void Label::Init(std::wstring text, Format textFormat, D2D1::ColorF::Enum color)
{
	mText = text;
	SetColor(color);
	SetFormat(textFormat);
}

void Label::SetText(std::wstring text)
{
	mText = text;
}

const auto & Label::GetText() const noexcept
{
	return mText;
}

void Label::SetColor(D2D1::ColorF::Enum color)
{
	size_t i = 0, size = mColors.size();
	for (; i < size; i++)
	{
		if (color == mColors[i])
		{
			break;
		}
	}
	if (i == size)
	{
		mColors.push_back(color);
		mColorBrushs.emplace_back();
		HR(md2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color),
			mColorBrushs[i].GetAddressOf()));//��������ɫ�Ļ�ˢ
	}
	mColorBrushIndex = i;
}

const auto & Label::GetColor() const noexcept
{
	return mColors[mColorBrushIndex];
}

void Label::SetFormat(Format & textFormat)
{
	//�ж������ʽ
	size_t i = 0, size = mFormats.size();
	for (; i < size; i++)				//Ѱ����û���Ѿ����ڵĸ�ʽ
	{
		if (mFormats[i] == textFormat)
		{
			break;
		}
	}
	if (i == size)							//û���ҵ�������ͬ�ĸ�ʽ
	{
		mFormats.push_back(textFormat);
		mTextFormats.emplace_back();
		HR(mdwriteFactory->CreateTextFormat(textFormat.font.c_str(), nullptr, textFormat.fontWeight,
			textFormat.fontStyle, textFormat.fontStretch, textFormat.fontSize, textFormat.localeName.c_str(),
			mTextFormats[i].GetAddressOf()));
	}
	mTextFormatIndex = i;					//ȷ����ʽλ��
}

const auto & Label::GetFormat() const noexcept
{
	return mFormats[mTextFormatIndex];
}

void Label::BeforeResize()
{
	for (auto & var : mColorBrushs)
	{
		var.Reset();
	}
	md2dRenderTarget.Reset();
}

void Label::AfterResize(const ComPtr<ID2D1Factory> & d2dFactory,
	const ComPtr<IDXGISwapChain> & swapChain)
{
	ComPtr<IDXGISurface> surface;
	HR(swapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HRESULT hr = d2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf());
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
		// �����̶���ɫˢ���ı���ʽ
		size_t i, size;
		for (i = 0, size = mColors.size(); i < size; i++)
		{
			HR(md2dRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(mColors[i]),
				mColorBrushs[i].GetAddressOf()));
		}
		for (i = 0, size = mTextFormats.size(); i < size; i++)
		{
			HR(mdwriteFactory->CreateTextFormat(mFormats[i].font.c_str(), nullptr, 
				mFormats[i].fontWeight,mFormats[i].fontStyle,mFormats[i].fontStretch, 
				mFormats[i].fontSize, mFormats[i].localeName.c_str(),
				mTextFormats[i].GetAddressOf()));
		}
	}
	else
	{
		// �����쳣����
		assert(md2dRenderTarget);
	}
}
