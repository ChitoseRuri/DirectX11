//Made by ChitoseRuri
//verson 0.9����ʱû�з���bug��
//�ⲿ����"DirectWrite.h"��"Sprite2D.h"
//�ؼ���ImageBox��TextBox��Button
//�����ؼ����UI�����඼����ʹ�ã�����Ӧ�����ĺ����ŵ���Ӧλ��

#ifndef UI_H_
#define UI_H_

#include "Sprite2D.h"
#include "DirectWrite.h"

namespace DX11UI
{
	enum ButtonStatus { BUTTON_OUT, BUTTON_ON, BUTTON_DOWN };

	class Control																																//�ؼ�����
	{
	public :
		Control();
		~Control();
		static void GetDevice(ID3D11Device * pDevice, ID3DX11Effect *pFX, ID3DX11EffectTechnique * pTechnique);
		//��ȡ����
		static void GetHwnd(HWND hwnd);
		//��ʼ���ؼ�
		virtual void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) = NULL;	
		//����D3D:ReSize֮ǰ	
		virtual void BeforeReSize() = NULL;		
		//����D3D:ReSize֮��
		virtual void AfterReSize() = NULL;					
		//�ؼ��ĸ���
		virtual void UpData() = NULL;	
		//���ƿؼ�
		virtual void Draw() = NULL;	
		//�ƶ��ؼ�
		virtual void Move(float x, float y) = NULL;		
		//�ؼ�����
		virtual void Visible();						
		//�ؼ�������
		virtual void Invisible();		

	protected:
		//�ؼ���4��״̬
		enum ControlStatus { ENABLE , DISABLE, VISIBLE, INVISIBLE };					
		static ID3D11Device * m_pD3dDevice;															
		static ID3DX11Effect *m_pFX;
		static ID3DX11EffectTechnique * m_pTechnique;
		static ID3D11BlendState* m_pAlphaToCoverageBS;
		static HWND m_Hwnd;
		int m_ControlStatus;																											//��ǿؼ���״̬
		unsigned order;
	};	



	class ImageBox :public Control
	{
	public:
		ImageBox();
		~ImageBox();
		//��ʼ���ؼ�
		static void Setup(ID3D11DeviceContext * d3DDeviceContext, ID3DX11EffectMatrixVariable* pWorld, 
			ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState);
		//����λ��
		void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) override;				
		//����ͼƬ
		void SetImage(char * path);																			
		//����D3D:ReSize֮ǰ	
		void BeforeReSize() override;												
		//����D3D:ReSize֮��
		void AfterReSize() override;			
		//�ؼ��ĸ���
		void UpData() override;		
		//�ƶ��ؼ�
		void Move(float x, float y) override;					
		//�ؼ���ת
		void Rotate(float radiam);																									
		//���ƿؼ�
		void Draw() override;																										

	private:
		Sprite2D m_Image;																											//ͼƬ�ؼ�
	};



	class TextBox :public Control
	{
	public:
		TextBox();
		~TextBox();
		//���뽻�����ʹ���
		static void Setup(IDXGISwapChain* pSwapChain, HWND hwnd);						
		//��ʼ���ؼ�
		void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) override;
		//��������
		void SetFont(wchar_t * wFont, FLOAT  fontSize);														
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords(const wchar_t * text, int color);																		
		//����D3D:ReSize֮ǰ	
		void BeforeReSize() override;																							
		//����D3D:ReSize֮��
		void AfterReSize() override;																							
		//�ؼ��ĸ���
		void UpData() override;																										
		//�ƶ��ؼ�
		void Move(float x, float y) override;																				
		//���ƿؼ�
		void Draw() override;																											

	private:
		static IDXGISwapChain * m_pSwapChain;
		DirectWrite m_Text;
		XMFLOAT2 m_LeftUp;
		XMFLOAT2 m_RightDown;
		wchar_t  m_pText[2048];
		int m_Color;
	};



	class Button:public Control
	{
	public:
		Button();
		~Button();
		//��ʼ���ؼ�
		static void Setup(IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
			ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState);
		//���ÿؼ�λ��
		void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) override;								
		void SetPicture_All(char * path);
		void SetFont_All(wchar_t * wFont, FLOAT  fontSize);
		void SetWords_All(wchar_t * text, int color);
		//����ͼƬ
		void SetPicture_ButtonOut(char * path);																		
		//��������
		void SetFont_ButtonOut(wchar_t * wFont, FLOAT  fontSize);							
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords_ButtonOut(wchar_t * text, int color);												
		//����ͼƬ
		void SetPicture_ButtonOn(char * path);																		
		//��������
		void SetFont_ButtonOn(wchar_t * wFont, FLOAT  fontSize);									
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords_ButtonOn(wchar_t * text, int color);													
		//����ͼƬ
		void SetPicture_ButtonDown(char * path);																	
		//��������
		void SetFont_ButtonDown(wchar_t * wFont, FLOAT  fontSize);							
		//����Ҫ��ʾ���ֺ���ɫ
		void SetWords_ButtonDown(wchar_t * text, int color);											
		//����D3D:ReSize֮ǰ	
		void BeforeReSize() override;																							
		//����D3D:ReSize֮��
		void AfterReSize() override;																								
		//�ؼ��ĸ���
		void UpData() override;																										
		//�ж�buttonʧ�����
		void ButtonCheck(const int &x, const int &y);
		//��갴��ʱ
		void ButtonDown();																											
		//���ſ�ʱ
		bool ButtonUp();																													
		//�ƶ��ؼ�
		void Move(float x, float y) override;																				
		//���ƿؼ�
		void Draw() override;																											
		//��������
		void Enable();																														
		//������������ͣ����OUT����̬
		void Disable();																														
		void Visible() override;
		void Invisible() override;

	private:
		void ReSetConButtonSize();
		static RECT m_Rect;
		ImageBox m_ButtonOut_Image;																					//��겻�ڸİ�ť��
		TextBox m_ButtonOut_Text;
		ImageBox m_ButtonOn_Image;																						//����ڰ�ť��
		TextBox m_ButtonOn_Text;
		ImageBox m_ButtonDown_Image;																				//��갴�°�ť
		TextBox m_ButtonDown_Text;
		XMFLOAT2 m_LeftUp;
		XMFLOAT2 m_RightDown;
		float m_ButtonLeft;
		float m_ButtonTop;
		float m_ButtonRight;
		float m_ButtonDown;
		int m_ButtonStatus;																											//button��״̬
	};		

	class UI
	{
	public:
		//����һ��ҳ����ֿؼ�������
		UI(int num_Textbox,int num_Picture,int num_Button);
		~UI();
		static void Setup(ID3D11Device * pDevice, ID3DX11Effect *pFX, ID3DX11EffectTechnique * pTechnique,
			IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
			ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout,ID3D11BlendState* pBlendState);
		//���ر�����Button���±�
		int ButtonCheck();
		//ʹ����Button�����
		void ResetButtonCheck();
		//����Updata����
		void Updata();
		//����d3dResizeǰ��
		void BeforeReSize();
		//����d3dResize����
		void AfterResize(HWND hwnd);
		//�������������µĵط�
		void MouseLeftDown();
		//�����������ſ��ط�
		void MouseLeftUp();
		//����
		void Draw();
		//��ȡ�ؼ����鳤��
		int Button_Size();
		int TextureBox_Size();
		int ImageBox_Size();

	public:
		TextBox* m_TextBox;
		ImageBox* m_ImageBox;
		Button* m_Button;

	private:
		static HWND m_Hwnd;
		int m_ButtonClick;
		int m_NumText, m_NumImage, m_NumButton;
	};
}
#endif // !UI_H_