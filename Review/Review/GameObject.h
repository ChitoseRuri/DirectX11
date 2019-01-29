#pragma once
#include "d3dApp.h"
#include <vector>

class GameObject
{
protected:
	GameObject();
	~GameObject();

public:
	//实例方法
	virtual void Init() = 0;
	virtual void DrawScene() = 0;
	virtual void UpdateScene(float dt) = 0;
	//静态方法
	static auto LoadTexture();

private:
	//静态共享成员
	static ComPtr<ID3D11Buffer> mVertexBuffer;							// 顶点缓冲区
	static ComPtr<ID3D11Buffer> mIndexBuffer;							// 索引缓冲区
	static std::vector<ComPtr<ID3D11ShaderResourceView>> mTextures;		// 纹理
	static UINT mVertexStride;											// 顶点字节大小
	static UINT mIndexCount;											// 索引数目	

	//实例成员
	DirectX::XMFLOAT4X4 mWorldMatrix;									// 世界矩阵
	



protected:
};
