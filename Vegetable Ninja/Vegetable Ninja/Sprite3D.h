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
	//初始化
	static void Setup3D(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX, ID3D11InputLayout *pLayout);
	static void Release();
	//加载网格
	static int LoadMesh(int meshType, char * path, ID3D11Buffer ** pVertexBuff, std::vector<Vertex3D> &vertices);
	//加载纹理
	static void LoadTexture(char * path, ID3D11ShaderResourceView ** pTexture);
	//设置顶点和定点数
	void SetVertex(ID3D11Buffer * pVertex, int num);
	//设置纹理
	void SetTexture(ID3D11ShaderResourceView * pTexture);
	//设置摄影机的矩阵
	void SetWorldProj(XMMATRIX &view, XMMATRIX &proj);
	//设置材质
	void SetMatrial(Material &mat);
	//平移
	void Move(XMFLOAT3 move);
	//平移到
	void MoveTo(XMFLOAT3 moveto);
	//绕z轴旋转
	void Rotate(float radium);
	//缩放
	void Scale(XMFLOAT3 scale);
	//设置初始平移值
	void SetReLocation(XMFLOAT3 move);
	//设置初始平移值
	void SetReLocation(float x, float y, float z);
	//设置初始缩放值
	void SetReScale(XMFLOAT3 scale);
	//设置初始缩放值
	void SetReScale(float x, float y, float z);
	//设置初始旋转值
	void SetReRolate(float rolate);
	//重设回初始设定状态
	void ReSetSprite();
	//绘制函数
	void Draw();
	//获得该物体的中心点
	XMFLOAT3 GetAveragePosition();
	//获得该物体的平均半径
	float GetRadius();
	//获得该物体的绝对平移信息
	XMFLOAT3  GetTranslationAbsolute();
	//获得该物体的相对平移信息
	XMFLOAT3 GetTranslationRelative();

public:
	//自定义储存信息
	int m_Flags;

private:
	//计算投影矩阵
	void CalculateWorldMatrix();
	//清理
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

