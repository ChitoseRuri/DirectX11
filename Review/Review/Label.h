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
		Format(std::wstring tfont = L"����",
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
	//��̬��Ա
	static std::vector<ComPtr<ID2D1SolidColorBrush>> mColorBrushs;				//��ɫ��ˢ
	static std::vector<D2D1::ColorF::Enum> mColors;								//������ɫ����
	static std::vector<ComPtr<IDWriteTextFormat>> mTextFormats;					//�ı���ʽ
	static std::vector<ComPtr<IDWriteFont>> mFonts;								//����
	static std::vector<Format> mFormats;										//������������

	//ʵ����Ա
	unsigned mColorBrushIndex;													//ʹ�õı�ˢ��λ��
	unsigned mTextFormatIndex;													//ʹ�õĸ�ʽ��λ��
	std::wstring mText;															//��Ҫ���Ƶ�����

public:
	//�ؼ�����
	Label();
	Label(std::wstring text, Format textFormat, D2D1::ColorF::Enum color);
	~Label();
	void OnResize() override;													//�����С�ı�����
	void DrawScene() override;													//����Label
	void UpdateScene(float dt) override;										//��ʱ����µ�����

	//Labelʵ������
	void Init(std::wstring & text, Format & textFormat, D2D1::ColorF::Enum color);
	void SetText(std::wstring text);
	const auto & GetText() const noexcept;
	void SetColor(D2D1::ColorF::Enum color);
	const auto & GetColor() const noexcept;
	void SetFormat(Format & textFormat);
	const auto & GetFormat() const noexcept;
	//Label��̬����
	static void BeforeResize();													//D3DApp::OnResize()ǰ����
	static void AfterResize();													//D3DApp::OnResize()�����
};