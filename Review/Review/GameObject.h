#pragma once
#include "d3dApp.h"
#include <vector>

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
	//��̬����
	static auto LoadTexture();

private:
	//��̬�����Ա
	static ComPtr<ID3D11Buffer> mVertexBuffer;							// ���㻺����
	static ComPtr<ID3D11Buffer> mIndexBuffer;							// ����������
	static std::vector<ComPtr<ID3D11ShaderResourceView>> mTextures;		// ����
	static UINT mVertexStride;											// �����ֽڴ�С
	static UINT mIndexCount;											// ������Ŀ	

	//ʵ����Ա
	DirectX::XMFLOAT4X4 mWorldMatrix;									// �������
	



protected:
};
