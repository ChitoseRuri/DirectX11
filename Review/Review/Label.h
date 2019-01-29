#pragma once
#include "Control.h"

class Label :public Control
{
public:
	struct Format
	{
	public:
		std::wstring font;
		DWRITE_FONT_WEIGHT fontWeight;
		DWRITE_FONT_STYLE fontStyle;
		DWRITE_FONT_STRETCH fontStretch;
		float fontSize;
		std::wstring localeName;

	public:
		Format(std::wstring tfont = L"宋体",
			DWRITE_FONT_WEIGHT tfontWeight = DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE tfontStyle = DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH tfontStretch = DWRITE_FONT_STRETCH_NORMAL,
			float tfontSize = 20.0,
			std::wstring tlocaleName = L"zh-cn"):
			font(tfont),
			fontWeight(tfontWeight),
			fontStyle(tfontStyle),
			fontStretch(tfontStretch),
			fontSize(tfontSize),
			localeName(tlocaleName)
		{}
		~Format() {}
		bool operator == (const Format & rhs) { return memcmp(this, &rhs, sizeof(Format)) == 0; }
	};
private:
	//静态成员
	static std::vector<ComPtr<ID2D1SolidColorBrush>> mColorBrushs;				//单色笔刷
	static std::vector<D2D1::ColorF::Enum> mColors;								//保存颜色数据
	static std::vector<ComPtr<IDWriteTextFormat>> mTextFormats;					//文本格式
	static std::vector<ComPtr<IDWriteFont>> mFonts;								//字体
	static std::vector<Format> mFormats;										//保存字体数据

	//实例成员
	unsigned mColorBrushIndex;													//使用的笔刷的位置
	unsigned mTextFormatIndex;													//使用的格式的位置
	std::wstring mText;															//需要绘制的文字

public:
	//控件方法
	Label();
	Label(std::wstring text, Format textFormat, D2D1::ColorF::Enum color);
	~Label();
	void OnResize() override;													//窗体大小改变后调用
	void DrawScene() override;													//绘制Label
	void UpdateScene(float dt) override;										//按时间更新的数据

	//Label实例方法
	void Init(std::wstring & text, Format & textFormat, D2D1::ColorF::Enum color);
	void SetText(std::wstring text);
	const auto & GetText() const noexcept;
	void SetColor(D2D1::ColorF::Enum color);
	const auto & GetColor() const noexcept;
	void SetFormat(Format & textFormat);
	const auto & GetFormat() const noexcept;
	//Label静态方法
	static void BeforeResize();													//D3DApp::OnResize()前调用
	static void AfterResize();													//D3DApp::OnResize()后调用
};