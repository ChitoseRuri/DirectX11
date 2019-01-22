#ifndef SPRITE2D_H
#define SPRITE2D_H

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

class Sprite2D
{
public:
	Sprite2D();
	~Sprite2D();
	static void Setup2D(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX, ID3DX11EffectTechnique * pTechnique,
		ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout,ID3D11BlendState * pBlendState);	
	static void SetDevice(ID3D11Device * d3DDevice);
	static void SetContext(ID3D11DeviceContext * d3DDeviceContext);
	static void SetFX(ID3DX11Effect * fX);
	void LoadPicture(char * path);
	void BuildGeometryBuffers(XMFLOAT2 leftUp,XMFLOAT2 rightDown, float deep);
	void BuildGeometryBuffers(ID3D11Buffer * buffer3D, unsigned num_vertex);
	void BuildFX();
	void BuildVertexLayout();
	void BuidTexture();
	void UpdateScene(float dt);
	void DrawScene();
	void Release();
	//ͼƬ��ת
	void ImgRotate(float angle);																												
	//ͼƬƽ��
	void ImgMove(float x, float y, float depth);	
	//ͼƬƽ��
	void ImgMove(float x, float y);
	//ͼƬƽ�Ƶ�
	void ImgMoveTo(float x, float y, float depth);
	//ͼƬƽ�Ƶ�
	void ImgMoveTo(float x, float y);
	//ͼƬ����
	void ImgScale(float x, float y, float z);																										
	//���ó�ʼƽ��ֵ
	void SetReLocation(XMFLOAT3 move);
	//���ó�ʼƽ��ֵ
	void SetReLocation(float x, float y, float z);
	//���ó�ʼ����ֵ
	void SetReScale(XMFLOAT3 scale);
	//���ó�ʼ����ֵ
	void SetReScale(float x, float y, float z);
	//���ó�ʼ��תֵ
	void SetReRolate(float rolate);
	//����س�ʼ�趨״̬
	void ReSetLocation();

private:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	static ID3D11Device * m_pD3DDevice;
	static ID3D11DeviceContext * m_pD3DDeviceContext;
	static ID3DX11Effect * m_pFX;
	static ID3DX11EffectTechnique * m_pTechnique;
	static ID3DX11EffectMatrixVariable* m_pWorld;
	static ID3D11InputLayout * m_pInputLayout;
	static ID3D11BlendState* m_pAlphaToCoverageBS;

	ID3D11ShaderResourceView * m_pShaderResourceView;
	LPD3D11EFFECTSHADERRESOURCEVARIABLE  m_pFXEffectShaderSourceVariable;
	ID3D11Buffer * m_pVertexBuff;
	ID3D11Buffer * m_pIndexBuff;
	
	XMFLOAT3 m_InitialLocation;
	XMFLOAT3 m_InitialScale;
	float m_InitialRotation;

	XMFLOAT3 m_Location;
	XMFLOAT3 m_Scale;
	float m_Rotation;

	XMMATRIX m_WorldMatrix;

	char m_Path[256];


	unsigned m_NumIndex;
};
#endif // !SPRITE2D_H
