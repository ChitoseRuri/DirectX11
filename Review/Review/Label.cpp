#include "Label.h"

using namespace DirectX;
using std::vector;
vector<ComPtr<ID2D1SolidColorBrush>> Label::mColorBrushs;				//单色笔刷
vector<D2D1::ColorF::Enum> Label::mColors;								//保存颜色数据
vector<ComPtr<IDWriteTextFormat>> Label::mTextFormats;					//文本格式
vector<ComPtr<IDWriteFont>> Label::mFonts;								//字体
vector<Label::Format> Label::mFormats;									//保存字体数据

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
			mColorBrushs[i].GetAddressOf()));//创建该颜色的画刷
	}
	mColorBrushIndex = i;
}

const auto & Label::GetColor() const noexcept
{
	return mColors[mColorBrushIndex];
}

void Label::SetFormat(Format & textFormat)
{
	//判断字体格式
	size_t i = 0, size = mFormats.size();
	for (; i < size; i++)				//寻找有没有已经存在的格式
	{
		if (mFormats[i] == textFormat)
		{
			break;
		}
	}
	if (i == size)							//没有找到存在相同的格式
	{
		mFormats.push_back(textFormat);
		mTextFormats.emplace_back();
		HR(mdwriteFactory->CreateTextFormat(textFormat.font.c_str(), nullptr, textFormat.fontWeight,
			textFormat.fontStyle, textFormat.fontStretch, textFormat.fontSize, textFormat.localeName.c_str(),
			mTextFormats[i].GetAddressOf()));
	}
	mTextFormatIndex = i;					//确定格式位置
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
		OutputDebugString(L"\n警告：Direct2D与Direct3D互操作性功能受限，你将无法看到文本信息。现提供下述可选方法：\n"
			"1. 对于Win7系统，需要更新至Win7 SP1，并安装KB2670838补丁以支持Direct2D显示。\n"
			"2. 自行完成Direct3D 10.1与Direct2D的交互。详情参阅："
			"https://docs.microsoft.com/zh-cn/windows/desktop/Direct2D/direct2d-and-direct3d-interoperation-overview""\n"
			"3. 使用别的字体库，比如FreeType。\n\n");
	}
	else if (hr == S_OK)
	{
		// 创建固定颜色刷和文本格式
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
		// 报告异常问题
		assert(md2dRenderTarget);
	}
}
