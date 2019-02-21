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
	//ʵ������
	virtual void Init() = 0;
	virtual void DrawScene() = 0;
	virtual void UpdateScene(float dt) = 0;

	void SetTexture(size_t index);
	void SetTexture(const ComPtr<ID3D11ShaderResourceView> & texture);
	//��̬����
	static void SetD3d(const ComPtr<ID3D11Device> & d3dDevice);			//����d3d�豸
	static const auto & LoadTexture(std::wstring path);					//������ͼ
	template<class VertexType, class IndexType>
	static void SetMeshData(const Geometry::MeshData<VertexType, IndexType>& meshData);

private:
	//��̬�����Ա
	static ComPtr<ID3D11Device> md3dDevice;								// D3D11�豸
	static std::vector<ComPtr<ID3D11ShaderResourceView>> mTextures;		// ����ָ������
	static ComPtr<ID3D11Buffer> mVertexBuffer;							// ���㻺����
	static ComPtr<ID3D11Buffer> mIndexBuffer;							// ����������
	static UINT mVertexStride;											// �����ֽڴ�С
	static UINT mIndexCount;											// ������Ŀ	
 
protected:
	//ʵ����Ա
	DirectX::XMFLOAT4X4 mWorldMatrix;									// �������
	ComPtr<ID3D11ShaderResourceView> mTexture;							// ����
	bool mIsVisable;													// �Ƿ����
	bool mIsDestory;													// �Ƿ��Ѿ��ͷţ����Ը��ǵ������ݣ�

protected:
};

template<class VertexType, class IndexType>
inline void GameObject::SetMeshData(const Geometry::MeshData<VertexType, IndexType>& meshData)
{
	// �ͷž���Դ
	mVertexBuffer.Reset();
	mIndexBuffer.Reset();

	// ���ö��㻺��������
	mVertexStride = sizeof(VertexType);
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * mVertexStride;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �½����㻺����
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(md3dDevice->CreateBuffer(&vbd, &InitData, mVertexBuffer.GetAddressOf()));


	// ������������������
	mIndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = mIndexCount * sizeof(IndexType);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �½�����������
	InitData.pSysMem = meshData.indexVec.data();
	HR(md3dDevice->CreateBuffer(&ibd, &InitData, mIndexBuffer.GetAddressOf()));



}
