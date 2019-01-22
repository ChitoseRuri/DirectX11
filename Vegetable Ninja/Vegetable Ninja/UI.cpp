#include "UI.h"

ID3D11Device * DX11UI::TextBox::m_pD3dDevice = nullptr;
IDXGISwapChain * DX11UI::TextBox::m_pSwapChain = nullptr;
ID3DX11Effect * DX11UI::Control::m_pFX = nullptr;
ID3DX11EffectTechnique *DX11UI::Control::m_pTechnique = nullptr;
ID3D11BlendState* DX11UI::Control::m_pAlphaToCoverageBS = nullptr;
HWND DX11UI::Control::m_Hwnd;
HWND DX11UI::UI::m_Hwnd;
RECT DX11UI::Button::m_Rect;

DX11UI::Control::Control()
{
	order = 0;
}

DX11UI::Control::~Control()
{
}

void DX11UI::Control::GetDevice(ID3D11Device * pDevice, ID3DX11Effect * pFX,  ID3DX11EffectTechnique * pTechnique)
{
	m_pD3dDevice = pDevice;
	m_pFX = pFX;
	m_pTechnique = pTechnique;
}

void DX11UI::Control::GetHwnd(HWND hwnd)
{
	m_Hwnd = hwnd;
}

void DX11UI::Control::Visible()
{
	m_ControlStatus = VISIBLE;
}

void DX11UI::Control::Invisible()
{
	m_ControlStatus = INVISIBLE;
}

DX11UI::Button::Button()
{
}

DX11UI::Button::~Button()
{
}

void DX11UI::Button::Setup(IDXGISwapChain * pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext, ID3DX11EffectMatrixVariable * pWorld, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState)
{
	ImageBox::Setup(d3DDeviceContext,  pWorld, pInputLayout,pBlendState);
	TextBox::Setup(pSwapChain, hwnd);
	GetClientRect(m_Hwnd, &m_Rect);
}

void DX11UI::Button::SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order)
{
	m_ButtonOut_Image.SetControl(leftUp, rightDown, order);
	m_ButtonOut_Text.SetControl(leftUp, rightDown, order);
	m_ButtonOn_Image.SetControl(leftUp, rightDown, order);
	m_ButtonOn_Text.SetControl(leftUp, rightDown, order);
	m_ButtonDown_Image.SetControl(leftUp, rightDown, order);
	m_ButtonDown_Text.SetControl(leftUp, rightDown, order);
	m_LeftUp = leftUp;
	m_RightDown = rightDown;
	ReSetConButtonSize();
	Visible();
	Enable();
	m_ButtonStatus = BUTTON_OUT;
}

void DX11UI::Button::SetPicture_All(char * path)
{
	SetPicture_ButtonOn(path);
	SetPicture_ButtonOut(path);
	SetPicture_ButtonDown(path);
}

void DX11UI::Button::SetFont_All(wchar_t * wFont, FLOAT fontSize)
{
	SetFont_ButtonOn(wFont, fontSize);
	SetFont_ButtonOut(wFont, fontSize);
	SetFont_ButtonDown(wFont, fontSize);
}

void DX11UI::Button::SetWords_All(wchar_t * text, int color)
{
	SetWords_ButtonOn(text, color);
	SetWords_ButtonOut(text, color);
	SetWords_ButtonDown(text, color);
}

void DX11UI::Button::Draw()
{
	switch (m_ButtonStatus)
	{
	case BUTTON_OUT:
	{
		if (m_ControlStatus != INVISIBLE)
		{
			m_ButtonOut_Image.Draw();
			m_ButtonOut_Text.Draw();
		}
		break;
	}
	case BUTTON_ON:
	{
		if (m_ControlStatus == ENABLE)
		{
			m_ButtonOn_Image.Draw();
			m_ButtonOn_Text.Draw();
		}
		else	if (m_ControlStatus != INVISIBLE)
		{
			m_ButtonOut_Image.Draw();
			m_ButtonOut_Text.Draw();
		}
		break;
	}
	case BUTTON_DOWN:
	{
		if (m_ControlStatus == ENABLE)
		{
			m_ButtonDown_Image.Draw();
			m_ButtonDown_Text.Draw();
		}
		else	if (m_ControlStatus != INVISIBLE)
		{
			m_ButtonOut_Image.Draw();
			m_ButtonOut_Text.Draw();
		}
		break;
	}
	default:
		break;
	}
}

void DX11UI::Button::Enable()
{
	m_ControlStatus = ENABLE;
}

void DX11UI::Button::Disable()
{
	m_ControlStatus = DISABLE;
}

void DX11UI::Button::Visible()
{
	m_ControlStatus = VISIBLE;
	m_ButtonOut_Image.Visible();
	m_ButtonOut_Text.Visible();
	m_ButtonOn_Image.Visible();
	m_ButtonOn_Text.Visible();
	m_ButtonDown_Image.Visible();
	m_ButtonDown_Text.Visible();
}

void DX11UI::Button::Invisible()
{
	m_ControlStatus = INVISIBLE;
	m_ButtonOut_Image.Invisible();
	m_ButtonOut_Text.Invisible();
	m_ButtonOn_Image.Invisible();
	m_ButtonOn_Text.Invisible();
	m_ButtonDown_Image.Invisible();
	m_ButtonDown_Text.Invisible();
}

void DX11UI::Button::ReSetConButtonSize()
{
	GetClientRect(m_Hwnd, &m_Rect);
	m_ButtonLeft = (m_LeftUp.x*0.5f + 0.5f)*m_Rect.right;
	m_ButtonDown = (-m_RightDown.y*0.5f + 0.5f)*m_Rect.bottom;
	m_ButtonRight = (m_RightDown.x*0.5f + 0.5f)*m_Rect.right;
	m_ButtonTop = (-m_LeftUp.y*0.5f + 0.5f)*m_Rect.bottom;
}

void DX11UI::Button::SetPicture_ButtonOut(char * path)
{
	m_ButtonOut_Image.SetImage(path);
}

void DX11UI::Button::SetFont_ButtonOut(wchar_t * wFont, FLOAT fontSize)
{
	m_ButtonOut_Text.SetFont(wFont, fontSize);
}

void DX11UI::Button::SetWords_ButtonOut(wchar_t * text, int color)
{
	m_ButtonOut_Text.SetWords(text, color);
}

void DX11UI::Button::SetPicture_ButtonOn(char * path)
{
	m_ButtonOn_Image.SetImage(path);
}

void DX11UI::Button::SetFont_ButtonOn(wchar_t * wFont, FLOAT fontSize)
{
	m_ButtonOn_Text.SetFont(wFont, fontSize);
}

void DX11UI::Button::SetWords_ButtonOn(wchar_t * text, int color)
{
	m_ButtonOn_Text.SetWords(text, color);
}

void DX11UI::Button::SetPicture_ButtonDown(char * path)
{
	m_ButtonDown_Image.SetImage(path);
}

void DX11UI::Button::SetFont_ButtonDown(wchar_t * wFont, FLOAT fontSize)
{
	m_ButtonDown_Text.SetFont(wFont, fontSize);
}

void DX11UI::Button::SetWords_ButtonDown(wchar_t * text, int color)
{
	m_ButtonDown_Text.SetWords(text, color);
}

void DX11UI::Button::BeforeReSize()
{
	m_ButtonOut_Text.BeforeReSize();
	m_ButtonOn_Text.BeforeReSize();
	m_ButtonDown_Text.BeforeReSize();
}

void DX11UI::Button::AfterReSize()
{
	m_ButtonOut_Text.AfterReSize();
	m_ButtonOn_Text.AfterReSize();
	m_ButtonDown_Text.AfterReSize();
	ReSetConButtonSize();
}

void DX11UI::Button::UpData()
{
}

void DX11UI::Button::ButtonCheck(const int &x, const int &y)
{
	if (x >= m_ButtonLeft&&x <= m_ButtonRight&&y >= m_ButtonTop&&y <= m_ButtonDown)
	{
		if (m_ButtonStatus == BUTTON_OUT)
		{
			m_ButtonStatus = BUTTON_ON;
		}
	}
	else
	{
		m_ButtonStatus = BUTTON_OUT;
	}
}

void DX11UI::Button::ButtonDown()
{
	if (m_ButtonStatus == BUTTON_ON)
	{
		m_ButtonStatus = BUTTON_DOWN;
	}
}

bool DX11UI::Button::ButtonUp()
{
	if (m_ButtonStatus == BUTTON_DOWN)
	{
		m_ButtonStatus = BUTTON_ON;
		return true;
	}
	return false;
}


void DX11UI::Button::Move(float x, float y)
{
	m_ButtonOut_Image.Move(x, y);
	m_ButtonOut_Text.Move(x, y);
	m_ButtonOn_Image.Move(x, y);
	m_ButtonOn_Text.Move(x, y);
	m_ButtonDown_Image.Move(x, y);
	m_ButtonDown_Text.Move(x, y);
	ReSetConButtonSize();
}

DX11UI::ImageBox::ImageBox()
{
}

DX11UI::ImageBox::~ImageBox()
{
}

void DX11UI::ImageBox::Setup(ID3D11DeviceContext * d3DDeviceContext, ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState)
{
	Sprite2D::Setup2D(m_pD3dDevice, d3DDeviceContext, m_pFX, m_pTechnique, pWorld, pInputLayout, pBlendState);
}

void DX11UI::ImageBox::SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order)
{
	m_Image.BuildGeometryBuffers(leftUp, rightDown, 1.0f - order*0.01f);
	m_ControlStatus = VISIBLE;
}

void DX11UI::ImageBox::SetImage(char * path)
{
	m_Image.LoadPicture(path);
}

void DX11UI::ImageBox::BeforeReSize()
{
}

void DX11UI::ImageBox::AfterReSize()
{
}

void DX11UI::ImageBox::UpData()
{
}

void DX11UI::ImageBox::Move(float x, float y)
{
	m_Image.ImgMove(x, y, 0.0f);
}



void DX11UI::ImageBox::Rotate(float radiam)
{
	m_Image.ImgRotate(radiam);
}

void DX11UI::ImageBox::Draw()
{
	if (m_ControlStatus == VISIBLE)
	{
		m_Image.DrawScene();
	}
}

DX11UI::TextBox::TextBox()
{
}

DX11UI::TextBox::~TextBox()
{
}

void DX11UI::TextBox::SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order)
{
	m_Text.Initialize(m_pSwapChain, m_Hwnd);
	m_LeftUp = leftUp;
	m_RightDown = rightDown;
}

void DX11UI::TextBox::Setup(IDXGISwapChain * pSwapChain, HWND hwnd)
{
	m_pSwapChain = pSwapChain;
	m_Hwnd = hwnd;
}

void DX11UI::TextBox::SetFont(wchar_t * wFont, FLOAT fontSize)
{
	m_Text.SetFont(wFont, fontSize);
	m_Text.SetTextAlignment();
	m_Text.SetParagraphAlignment();
}

void DX11UI::TextBox::SetWords(const wchar_t *text, int color)
{
	if (text != nullptr)
	{
		ZeroMemory(m_pText, 2048);
		lstrcpyW(m_pText, text);
	}
	m_Color = color;
}

void DX11UI::TextBox::BeforeReSize()
{
	m_Text.PrepareResize();
}

void DX11UI::TextBox::AfterReSize()
{
	m_Text.SetDigxBuff();
}

void DX11UI::TextBox::UpData()
{
}

void DX11UI::TextBox::Move(float x, float y)
{
	m_LeftUp.x -= x;
	m_LeftUp.y -= y;
	m_RightDown.x -= x;
	m_RightDown.y -= y;
}

void DX11UI::TextBox::Draw()
{
	m_Text.Draw(m_pText, m_Color, m_LeftUp, m_RightDown);
}

DX11UI::UI::UI(int num_Textbox, int num_Picture, int num_Button)
{
	m_ButtonClick = -1;
	m_TextBox = new TextBox[num_Textbox];
	m_ImageBox = new ImageBox[num_Picture];
	m_Button = new Button[num_Button];
	m_NumText = num_Textbox;
	m_NumImage = num_Picture;
	m_NumButton = num_Button;
}

DX11UI::UI::~UI()
{
	delete[] m_TextBox;
	delete[] m_ImageBox;
	delete[] m_Button;
}

void DX11UI::UI::Setup(ID3D11Device * pDevice, ID3DX11Effect *pFX, ID3DX11EffectTechnique * pTechnique,
	IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
	ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState)
{
	Control::GetDevice(pDevice, pFX, pTechnique);
	Control::GetHwnd(hwnd);
	m_Hwnd = hwnd;
	Button::Setup(pSwapChain, hwnd, d3DDeviceContext, pWorld, pInputLayout, pBlendState);
}

int DX11UI::UI::ButtonCheck()
{
	return m_ButtonClick;
}

void DX11UI::UI::ResetButtonCheck()
{
	m_ButtonClick = -1;
}

void DX11UI::UI::Updata()
{
	POINT Cursor;
	GetCursorPos(&Cursor);
	ScreenToClient(m_Hwnd, &Cursor);
	for (int i = 0;i<m_NumButton;i++)
	{
		m_Button[i].ButtonCheck(Cursor.x, Cursor.y);
	}
}

void DX11UI::UI::BeforeReSize()
{
	if (this != nullptr)
	{
		for (int i = 0; i<m_NumText; i++)
		{
			m_TextBox[i].BeforeReSize();
		}
		for (int i = 0; i<m_NumImage; i++)
		{
			m_ImageBox[i].BeforeReSize();
		}
		for (int i = 0; i<m_NumButton; i++)
		{
			m_Button[i].BeforeReSize();
		}
	}
}

void DX11UI::UI::AfterResize(HWND hwnd)
{
	if (this != nullptr)
	{
		m_Hwnd = hwnd;
		Control::GetHwnd(hwnd);
		for (int i = 0; i<m_NumText; i++)
		{
			m_TextBox[i].AfterReSize();
		}
		for (int i = 0; i<m_NumImage; i++)
		{
			m_ImageBox[i].AfterReSize();
		}
		for (int i = 0; i<m_NumButton; i++)
		{
			m_Button[i].AfterReSize();
		}
	}
}

void DX11UI::UI::MouseLeftDown()
{
	for (int i = 0;i<m_NumButton;i++)
	{
		m_Button[i].ButtonDown();
	}
}

void DX11UI::UI::MouseLeftUp()
{
	for (int i = 0;i<m_NumButton;i++)
	{
		if (m_Button[i].ButtonUp())
		{
			m_ButtonClick = i;
			break;
		}
	}
}

void DX11UI::UI::Draw()
{
	for (int i = 0; i<m_NumImage; i++)
	{
		m_ImageBox[i].Draw();
	}
	for (int i = 0; i<m_NumText; i++)
	{
		m_TextBox[i].Draw();
	}
	for (int i = 0; i<m_NumButton; i++)
	{
		m_Button[i].Draw();
	}
}

int DX11UI::UI::Button_Size()
{
	return m_NumButton;
}

int DX11UI::UI::TextureBox_Size()
{
	return m_NumText;
}

int DX11UI::UI::ImageBox_Size()
{
	return m_NumImage;
}
