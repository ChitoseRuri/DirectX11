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

Label::Label(std::wstring text, Format textFormat, D2D1::ColorF::Enum color)
{
	Init(text, textFormat, color);
}

Label::~Label()
{
}

void Label::OnResize()
{
}

void Label::DrawScene()
{
	md2dRenderTarget->DrawTextW(mText.c_str(), mText.length(), mTextFormats[mTextFormatIndex].Get(),
		mRectD, mColorBrushs[mColorBrushIndex].Get());
}

void Label::UpdateScene(float dt)
{
}

void Label::Init(std::wstring & text, Format & textFormat, D2D1::ColorF::Enum color)
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
			mFormats[i].fontWeight, mFormats[i].fontStyle, mFormats[i].fontStretch,
			mFormats[i].fontSize, mFormats[i].localeName.c_str(),
			mTextFormats[i].GetAddressOf()));
	}
}
