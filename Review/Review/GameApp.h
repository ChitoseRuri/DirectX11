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

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();


private:
	bool InitEffect();
	bool InitResource();



private:
	ComPtr<ID3D11InputLayout> mVertexLayout;	// �������벼��
	ComPtr<ID3D11Buffer> mVertexBuffer;			// ���㻺����
	ComPtr<ID3D11VertexShader> mVertexShader;	// ������ɫ��
	ComPtr<ID3D11PixelShader> mPixelShader;		// ������ɫ��
};