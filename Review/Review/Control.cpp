#include "Control.h"

using namespace DirectX;

ComPtr<IDWriteFactory> Control::mdwriteFactory;				//DWrite工厂
ComPtr<ID2D1RenderTarget> Control::md2dRenderTarget;		//D2D渲染目标

Control::Control() :
	mRotation({ 0.0f,0.0f }),
	mRectN({ 0.0f,0.0f,0.0f,0.0f }),
	mRectD({ 0.0f,0.0f,0.0f,0.0f }),
	mIsVisable(true)
{
}

Control::~Control()
{
}

void Control::SetVisable(bool isviable)
{
	mIsVisable = isviable;
}

bool Control::GetVisable() const noexcept
{
	return mIsVisable;
}

void Control::SetRect(D2D_RECT_F rect)
{
	mRectN = rect;
}

const D2D_RECT_F & Control::GetRect() const noexcept
{
	return mRectN;
}

void Control::SetRotation(DirectX::XMFLOAT2 rotation)
{
	mRotation = rotation;
}

const DirectX::XMFLOAT2 & Control::GetRotation() const noexcept
{
	return mRotation;
}

void Control::InitControl(const ComPtr<IDWriteFactory>& dwriteFactory,
	const ComPtr<ID2D1RenderTarget> d2dRenderTarget)
{
	mdwriteFactory = dwriteFactory;
	md2dRenderTarget = d2dRenderTarget;
}
