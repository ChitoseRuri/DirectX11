#pragma once
#include "d3dApp.h"
#include "Geometry.h"
#include "LightHelper.h"
#include "Camera.h"

	
class GameApp : public D3DApp
{
public:
	struct VertexPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
		static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
	};

public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init() override;
	bool InitInput() override;
	bool InitUI();
	void OnResize()	override;
	void UpdateScene(float dt) override;
	void DrawScene() override;


private:
	bool InitEffect();
	bool InitResource();


private:
	ComPtr<ID3D11InputLayout> mVertexLayout;	// 顶点输入布局
	ComPtr<ID3D11Buffer> mVertexBuffer;			// 顶点缓冲区
	ComPtr<ID3D11VertexShader> mVertexShader;	// 顶点着色器
	ComPtr<ID3D11PixelShader> mPixelShader;		// 像素着色器
	
	ComPtr<ID2D1SolidColorBrush> mColorBrush;	//单色笔刷
	ComPtr<IDWriteFont> mFont;					//字体
	ComPtr<IDWriteTextFormat> mTextFormat;		//文本格式


};