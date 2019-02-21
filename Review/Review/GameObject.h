#pragma once
#include "d3dApp.h"
#include <vector>
#include "DDSTextureLoader.h"
#include "Geometry.h"


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

	void SetTexture(size_t index);
	void SetTexture(const ComPtr<ID3D11ShaderResourceView> & texture);
	//静态方法
	static void SetD3d(const ComPtr<ID3D11Device> & d3dDevice);			//传递d3d设备
	static const auto & LoadTexture(std::wstring path);					//加载贴图
	template<class VertexType, class IndexType>
	static void SetMeshData(const Geometry::MeshData<VertexType, IndexType>& meshData);

private:
	//静态共享成员
	static ComPtr<ID3D11Device> md3dDevice;								// D3D11设备
	static std::vector<ComPtr<ID3D11ShaderResourceView>> mTextures;		// 纹理指针容器
	static ComPtr<ID3D11Buffer> mVertexBuffer;							// 顶点缓冲区
	static ComPtr<ID3D11Buffer> mIndexBuffer;							// 索引缓冲区
	static UINT mVertexStride;											// 顶点字节大小
	static UINT mIndexCount;											// 索引数目	
 
protected:
	//实例成员
	DirectX::XMFLOAT4X4 mWorldMatrix;									// 世界矩阵
	ComPtr<ID3D11ShaderResourceView> mTexture;							// 纹理
	bool mIsVisable;													// 是否绘制
	bool mIsDestory;													// 是否已经释放（可以覆盖掉该数据）

protected:
};

template<class VertexType, class IndexType>
inline void GameObject::SetMeshData(const Geometry::MeshData<VertexType, IndexType>& meshData)
{
	// 释放旧资源
	mVertexBuffer.Reset();
	mIndexBuffer.Reset();

	// 设置顶点缓冲区描述
	mVertexStride = sizeof(VertexType);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * mVertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(md3dDevice->CreateBuffer(&vbd, &InitData, mVertexBuffer.GetAddressOf()));


	// 设置索引缓冲区描述
	mIndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = mIndexCount * sizeof(IndexType);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(md3dDevice->CreateBuffer(&ibd, &InitData, mIndexBuffer.GetAddressOf()));



}
