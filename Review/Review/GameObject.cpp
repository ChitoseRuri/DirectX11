#include "GameApp.h"

//��̬�����Ա
ComPtr<ID3D11Device> GameObject::md3dDevice;							// D3D11�豸
ComPtr<ID3D11Buffer> GameObject::mVertexBuffer;							// ���㻺����
ComPtr<ID3D11Buffer> GameObject::mIndexBuffer;							// ����������
std::vector<ComPtr<ID3D11ShaderResourceView>> GameObject::mTextures;	// ����
UINT GameObject::mVertexStride;											// �����ֽڴ�С
UINT GameObject::mIndexCount;											// ������Ŀ	

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


