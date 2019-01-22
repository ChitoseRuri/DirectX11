//Made by ChitoseRuri
//verson 0.9（暂时没有发现bug）
//外部包含"DirectWrite.h"、"Sprite2D.h"
//控件：ImageBox、TextBox、Button
//三个控件类和UI汇总类都可以使用，把相应描述的函数放到对应位置

#ifndef UI_H_
#define UI_H_

#include "Sprite2D.h"
#include "DirectWrite.h"

namespace DX11UI
{
	enum ButtonStatus { BUTTON_OUT, BUTTON_ON, BUTTON_DOWN };

	class Control																																//控件基类
	{
	public :
		Control();
		~Control();
		static void GetDevice(ID3D11Device * pDevice, ID3DX11Effect *pFX, ID3DX11EffectTechnique * pTechnique);
		//获取窗口
		static void GetHwnd(HWND hwnd);
		//初始化控件
		virtual void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) = NULL;	
		//放在D3D:ReSize之前	
		virtual void BeforeReSize() = NULL;		
		//放在D3D:ReSize之后
		virtual void AfterReSize() = NULL;					
		//控件的更新
		virtual void UpData() = NULL;	
		//绘制控件
		virtual void Draw() = NULL;	
		//移动控件
		virtual void Move(float x, float y) = NULL;		
		//控件可视
		virtual void Visible();						
		//控件不可视
		virtual void Invisible();		

	protected:
		//控件的4种状态
		enum ControlStatus { ENABLE , DISABLE, VISIBLE, INVISIBLE };					
		static ID3D11Device * m_pD3dDevice;															
		static ID3DX11Effect *m_pFX;
		static ID3DX11EffectTechnique * m_pTechnique;
		static ID3D11BlendState* m_pAlphaToCoverageBS;
		static HWND m_Hwnd;
		int m_ControlStatus;																											//标记控件的状态
		unsigned order;
	};	



	class ImageBox :public Control
	{
	public:
		ImageBox();
		~ImageBox();
		//初始化控件
		static void Setup(ID3D11DeviceContext * d3DDeviceContext, ID3DX11EffectMatrixVariable* pWorld, 
			ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState);
		//设置位置
		void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) override;				
		//设置图片
		void SetImage(char * path);																			
		//放在D3D:ReSize之前	
		void BeforeReSize() override;												
		//放在D3D:ReSize之后
		void AfterReSize() override;			
		//控件的更新
		void UpData() override;		
		//移动控件
		void Move(float x, float y) override;					
		//控件旋转
		void Rotate(float radiam);																									
		//绘制控件
		void Draw() override;																										

	private:
		Sprite2D m_Image;																											//图片控件
	};



	class TextBox :public Control
	{
	public:
		TextBox();
		~TextBox();
		//传入交换链和窗口
		static void Setup(IDXGISwapChain* pSwapChain, HWND hwnd);						
		//初始化控件
		void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) override;
		//设置字体
		void SetFont(wchar_t * wFont, FLOAT  fontSize);														
		//设置要显示的字和颜色
		void SetWords(const wchar_t * text, int color);																		
		//放在D3D:ReSize之前	
		void BeforeReSize() override;																							
		//放在D3D:ReSize之后
		void AfterReSize() override;																							
		//控件的更新
		void UpData() override;																										
		//移动控件
		void Move(float x, float y) override;																				
		//绘制控件
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
		//初始化控件
		static void Setup(IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
			ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState);
		//设置控件位置
		void SetControl(XMFLOAT2 leftUp, XMFLOAT2 rightDown, unsigned order) override;								
		void SetPicture_All(char * path);
		void SetFont_All(wchar_t * wFont, FLOAT  fontSize);
		void SetWords_All(wchar_t * text, int color);
		//设置图片
		void SetPicture_ButtonOut(char * path);																		
		//设置字体
		void SetFont_ButtonOut(wchar_t * wFont, FLOAT  fontSize);							
		//设置要显示的字和颜色
		void SetWords_ButtonOut(wchar_t * text, int color);												
		//设置图片
		void SetPicture_ButtonOn(char * path);																		
		//设置字体
		void SetFont_ButtonOn(wchar_t * wFont, FLOAT  fontSize);									
		//设置要显示的字和颜色
		void SetWords_ButtonOn(wchar_t * text, int color);													
		//设置图片
		void SetPicture_ButtonDown(char * path);																	
		//设置字体
		void SetFont_ButtonDown(wchar_t * wFont, FLOAT  fontSize);							
		//设置要显示的字和颜色
		void SetWords_ButtonDown(wchar_t * text, int color);											
		//放在D3D:ReSize之前	
		void BeforeReSize() override;																							
		//放在D3D:ReSize之后
		void AfterReSize() override;																								
		//控件的更新
		void UpData() override;																										
		//判断button失焦情况
		void ButtonCheck(const int &x, const int &y);
		//鼠标按下时
		void ButtonDown();																											
		//鼠标放开时
		bool ButtonUp();																													
		//移动控件
		void Move(float x, float y) override;																				
		//绘制控件
		void Draw() override;																											
		//按键启用
		void Enable();																														
		//按键不启动，停留在OUT的形态
		void Disable();																														
		void Visible() override;
		void Invisible() override;

	private:
		void ReSetConButtonSize();
		static RECT m_Rect;
		ImageBox m_ButtonOut_Image;																					//鼠标不在改按钮上
		TextBox m_ButtonOut_Text;
		ImageBox m_ButtonOn_Image;																						//鼠标在按钮上
		TextBox m_ButtonOn_Text;
		ImageBox m_ButtonDown_Image;																				//鼠标按下按钮
		TextBox m_ButtonDown_Text;
		XMFLOAT2 m_LeftUp;
		XMFLOAT2 m_RightDown;
		float m_ButtonLeft;
		float m_ButtonTop;
		float m_ButtonRight;
		float m_ButtonDown;
		int m_ButtonStatus;																											//button的状态
	};		

	class UI
	{
	public:
		//填上一个页面各种控件的数量
		UI(int num_Textbox,int num_Picture,int num_Button);
		~UI();
		static void Setup(ID3D11Device * pDevice, ID3DX11Effect *pFX, ID3DX11EffectTechnique * pTechnique,
			IDXGISwapChain* pSwapChain, HWND hwnd, ID3D11DeviceContext * d3DDeviceContext,
			ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout,ID3D11BlendState* pBlendState);
		//返回被按的Button的下标
		int ButtonCheck();
		//使用完Button后调用
		void ResetButtonCheck();
		//放在Updata里面
		void Updata();
		//放在d3dResize前面
		void BeforeReSize();
		//放在d3dResize后面
		void AfterResize(HWND hwnd);
		//放在鼠标左键按下的地方
		void MouseLeftDown();
		//放在鼠标左键放开地方
		void MouseLeftUp();
		//绘制
		void Draw();
		//获取控件数组长度
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