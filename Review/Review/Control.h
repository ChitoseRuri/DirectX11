#pragma once
#include <wrl/client.h>
#include <string>
#include <set>
#include <vector>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include "DXTrace.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// 使用模板别名(C++11)简化类型名
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

//抽象基类
class Control
{
protected:
	//保护成员
	DirectX::XMFLOAT2 mRotation;	//旋转角度
	D2D_RECT_F mRectN;				//原始Rect
	D2D_RECT_F mRectD;				//绘制Rect
	bool mIsVisable;				//是否绘制

	static ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite工厂
	static ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D渲染目标

protected:
	Control();
	~Control();

public:
	//实例方法
	virtual void OnResize() = 0;												//窗体大小改变后调用
	virtual void DrawScene() = 0;												//绘制Label
	virtual void UpdateScene(float dt) = 0;										//按时间更新的数据
	virtual void SetVisable(bool isviable);										//设置是否可视
	virtual bool GetVisable() const noexcept;									//获取visable
	virtual void SetRect(D2D_RECT_F rect);										//设置rect
	virtual const D2D_RECT_F & GetRect() const noexcept;						//获取rect
	virtual void SetRotation(DirectX::XMFLOAT2 rotation);						//设置旋转
	virtual const DirectX::XMFLOAT2 & GetRotation() const noexcept;				//获取旋转

	//静态方法
	static void InitControl(const ComPtr<IDWriteFactory> & dwriteFactory,
		const ComPtr<ID2D1RenderTarget> & d2dRenderTarget);						//静态初始化
};
