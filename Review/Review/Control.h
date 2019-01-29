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

// ʹ��ģ�����(C++11)��������
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

//�������
class Control
{
protected:
	//������Ա
	DirectX::XMFLOAT2 mRotation;	//��ת�Ƕ�
	D2D_RECT_F mRectN;				//ԭʼRect
	D2D_RECT_F mRectD;				//����Rect
	bool mIsVisable;				//�Ƿ����

	static ComPtr<IDWriteFactory> mdwriteFactory;				//DWrite����
	static ComPtr<ID2D1RenderTarget> md2dRenderTarget;			//D2D��ȾĿ��

protected:
	Control();
	~Control();

public:
	//ʵ������
	virtual void OnResize() = 0;												//�����С�ı�����
	virtual void DrawScene() = 0;												//����Label
	virtual void UpdateScene(float dt) = 0;										//��ʱ����µ�����
	virtual void SetVisable(bool isviable);										//�����Ƿ����
	virtual bool GetVisable() const noexcept;									//��ȡvisable
	virtual void SetRect(D2D_RECT_F rect);										//����rect
	virtual const D2D_RECT_F & GetRect() const noexcept;						//��ȡrect
	virtual void SetRotation(DirectX::XMFLOAT2 rotation);						//������ת
	virtual const DirectX::XMFLOAT2 & GetRotation() const noexcept;				//��ȡ��ת

	//��̬����
	static void InitControl(const ComPtr<IDWriteFactory> & dwriteFactory,
		const ComPtr<ID2D1RenderTarget> & d2dRenderTarget);						//��̬��ʼ��
};
