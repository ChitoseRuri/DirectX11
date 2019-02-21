#include "GameApp.h"

//静态共享成员
ComPtr<ID3D11Device> GameObject::md3dDevice;							// D3D11设备
ComPtr<ID3D11Buffer> GameObject::mVertexBuffer;							// 顶点缓冲区
ComPtr<ID3D11Buffer> GameObject::mIndexBuffer;							// 索引缓冲区
std::vector<ComPtr<ID3D11ShaderResourceView>> GameObject::mTextures;	// 纹理
UINT GameObject::mVertexStride;											// 顶点字节大小
UINT GameObject::mIndexCount;											// 索引数目	

using namespace DirectX;

GameObject::GameObject() :
	mTexture(nullptr),
	mIsVisable(true),
	mIsDestory(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetTexture(size_t index)
{
	assert(index < mTextures.size());
	mTexture = mTextures[index];
}

void GameObject::SetTexture(const ComPtr<ID3D11ShaderResourceView>& texture)
{
	assert(texture != nullptr);
	mTexture = texture;
}

void GameObject::SetD3d(const ComPtr<ID3D11Device>& d3dDevice)
{
	md3dDevice = d3dDevice;
}

const auto & GameObject::LoadTexture(std::wstring path)
{
	mTextures.emplace_back();
	CreateDDSTextureFromFile(md3dDevice.Get(), path.c_str(), nullptr, mTextures.back().GetAddressOf());
	return mTextures.back();
}


