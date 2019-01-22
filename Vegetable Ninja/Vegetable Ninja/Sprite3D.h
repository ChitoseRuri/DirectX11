#ifndef SPRITE_3D_H
#define SPRITE_3D_H

#include "d3dUtil.h"
#include <vector>

struct Vertex3D
{
	Vertex3D() {};
	Vertex3D(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT2& uv)
		: Pos(p), Normal(n), Texture(uv) {}
	Vertex3D(float px, float py, float pz,
		float nx, float ny, float nz,
		float u, float v) :Pos(px, py, pz), Normal(nx, ny, nz), Texture(u, v) {};
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Texture;
};

struct Material
{
	XMFLOAT4	ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
};

enum MeshType{OBJ,M3D};

class Sprite3D
{
public:
	Sprite3D();
	~Sprite3D();
	//��ʼ��
	static void Setup3D(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX, ID3D11InputLayout *pLayout);
	static void Release();
	//��������
	static int LoadMesh(int meshType, char * path, ID3D11Buffer ** pVertexBuff, std::vector<Vertex3D> &vertices);
	//��������
	static void LoadTexture(char * path, ID3D11ShaderResourceView ** pTexture);
	//���ö���Ͷ�����
	void SetVertex(ID3D11Buffer * pVertex, int num);
	//��������
	void SetTexture(ID3D11ShaderResourceView * pTexture);
	//������Ӱ���ľ���
	void SetWorldProj(XMMATRIX &view, XMMATRIX &proj);
	//���ò���
	void SetMatrial(Material &mat);
	//ƽ��
	void Move(XMFLOAT3 move);
	//ƽ�Ƶ�
	void MoveTo(XMFLOAT3 moveto);
	//��z����ת
	void Rotate(float radium);
	//����
	void Scale(XMFLOAT3 scale);
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
	void ReSetSprite();
	//���ƺ���
	void Draw();
	//��ø���������ĵ�
	XMFLOAT3 GetAveragePosition();
	//��ø������ƽ���뾶
	float GetRadius();
	//��ø�����ľ���ƽ����Ϣ
	XMFLOAT3  GetTranslationAbsolute();
	//��ø���������ƽ����Ϣ
	XMFLOAT3 GetTranslationRelative();

public:
	//�Զ��崢����Ϣ
	int m_Flags;

private:
	//����ͶӰ����
	void CalculateWorldMatrix();
	//����
	void ClearCom();

private:
	static ID3D11Device * m_pD3DDevice;
	static ID3D11DeviceContext * m_pD3DDeviceContext;
	static ID3DX11Effect * m_pEffect;
	static ID3DX11EffectTechnique * m_pTechnique;
	static ID3DX11EffectMatrixVariable* m_pWorld;
	static ID3D11InputLayout * m_pInputLayout;
	static ID3DX11EffectTechnique* m_LightTechnique;
	static ID3DX11EffectMatrixVariable* m_pEffectWorldViewProj;
	static ID3DX11EffectMatrixVariable* m_pEffectWorld;
	static ID3DX11EffectMatrixVariable* m_pTexTransform;
	static ID3DX11EffectMatrixVariable* m_pEffectWorldInvTranspose;
	static ID3DX11EffectVectorVariable* m_pEffectEyePosW;
	static LPD3D11EFFECTSHADERRESOURCEVARIABLE m_pEffectShaderResourceVariable;
	static ID3DX11EffectVariable* m_pEffectMaterial;

	ID3D11BlendState* m_pBlendState;
	ID3D11ShaderResourceView * m_ShaderResourceView;
	unsigned m_NumIndex;
	Material m_Material;
	ID3D11Buffer * m_pVertexBuff;
	ID3D11Device * m_D3DDevice;
	XMMATRIX m_ModelWorld;
	XMMATRIX m_WorldProj;
	XMFLOAT3 m_InitialLocation;
	XMFLOAT3 m_InitialScale;
	float m_InitialRotation;

	XMFLOAT3 m_Location;
	XMFLOAT3 m_Scale;
	float m_Rotation;

	XMFLOAT3 m_AveragePosition;
	float m_Radius;
	UINT stride;
	UINT offset;


};
#endif // !SPRITE_3D_H

