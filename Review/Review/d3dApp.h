#pragma once
#include <wrl/client.h>
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "GameTimer.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "UI.h"

// 添加所有要引用的库
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "winmm.lib")

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);    // 在构造函数的初始化列表应当设置好初始参数
	virtual ~D3DApp();

	HINSTANCE AppInst()const;       // 获取应用实例的句柄
	HWND      MainWnd()const;       // 获取主窗口句柄
	float     AspectRatio()const;   // 获取屏幕宽高比

	int Run();                      // 运行程序，进行游戏主循环

	// 框架方法。客户派生类需要重载这些方法以实现特定的应用需求
	virtual bool Init();            // 该父类方法需要初始化窗口和Direct3D部分
	virtual bool InitInput() = 0;	//初始化需要的控制器
	virtual void OnResize();        // 该父类方法需要在窗口大小变动的时候调用
	virtual void UpdateScene(float dt) = 0;   // 子类需要实现该方法，完成每一帧的更新
	virtual void DrawScene() = 0;             // 子类需要实现该方法，完成每一帧的绘制
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	 // 窗口的消息回调函数

protected:
	virtual bool InitMainWindow();      // 窗口初始化
	//virtual bool InitDirect2D();		// D2D初始化
	virtual bool InitDirect3D();        // Direct3D初始化

	void CalculateFrameStats(); // 计算每秒帧数并在窗口显示

protected:

	HINSTANCE mhAppInst;        // 应用实例句柄
	HWND      mhMainWnd;        // 主窗口句柄
	bool      mAppPaused;       // 应用是否暂停
	bool      mMinimized;       // 应用是否最小化
	bool      mMaximized;       // 应用是否最大化
	bool      mResizing;        // 窗口大小是否变化
	bool	  mEnable4xMsaa;	// 是否开启4倍多重采样
	UINT      m4xMsaaQuality;   // MSAA支持的质量等级


	GameTimer mTimer;           // 计时器

	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	////D2D
	//ComPtr<ID2D1Factory> md2dFactory;					//D2D工厂
	//ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite工厂
	//ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D渲染目标
	// DX11
	ComPtr<ID3D11Device> md3dDevice;                    // D3D11设备
	ComPtr<ID3D11DeviceContext> md3dImmediateContext;   // D3D11设备上下文
	ComPtr<IDXGISwapChain> mSwapChain;                  // D3D11交换链
	// DX11.1
	ComPtr<ID3D11Device1> md3dDevice1;                  // D3D11.1设备
	ComPtr<ID3D11DeviceContext1> md3dImmediateContext1; // D3D11.1设备上下文
	ComPtr<IDXGISwapChain1> mSwapChain1;                // D3D11.1交换链
	// 常用资源
	ComPtr<ID3D11Texture2D> mDepthStencilBuffer;        // 深度模板缓冲区
	ComPtr<ID3D11RenderTargetView> mRenderTargetView;   // 渲染目标视图
	ComPtr<ID3D11DepthStencilView> mDepthStencilView;   // 深度模板视图
	D3D11_VIEWPORT mScreenViewport;                     // 视口

	// 派生类应该在构造函数设置好这些自定义的初始参数
	std::wstring mMainWndCaption;                       // 主窗口标题
	int mClientWidth;                                   // 视口宽度
	int mClientHeight;                                  // 视口高度

	//鼠标键盘
	std::unique_ptr<DirectX::Mouse> mMouse;						//鼠标“单例”
	DirectX::Mouse::ButtonStateTracker mMouseTracker;			//鼠标(按钮)状态追踪器
	std::unique_ptr<DirectX::Keyboard> mKeyboard;				//键盘“单例”
	DirectX::Keyboard::KeyboardStateTracker mKeyboardTracker;	//键盘状态追踪器

	UI mUI;														//UI类测试
};