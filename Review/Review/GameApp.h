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
	ComPtr<ID3D11InputLayout> mVertexLayout;	// �������벼��
	ComPtr<ID3D11Buffer> mVertexBuffer;			// ���㻺����
	ComPtr<ID3D11VertexShader> mVertexShader;	// ������ɫ��
	ComPtr<ID3D11PixelShader> mPixelShader;		// ������ɫ��
	
	ComPtr<ID2D1SolidColorBrush> mColorBrush;	//��ɫ��ˢ
	ComPtr<IDWriteFont> mFont;					//����
	ComPtr<IDWriteTextFormat> mTextFormat;		//�ı���ʽ


};