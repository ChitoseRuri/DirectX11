#include "GameApp.h"

//��̬�����Ա
ComPtr<ID3D11Buffer> GameObject::mVertexBuffer;							// ���㻺����
ComPtr<ID3D11Buffer> GameObject::mIndexBuffer;							// ����������
std::vector<ComPtr<ID3D11ShaderResourceView>> GameObject::mTextures;	// ����
UINT GameObject::mVertexStride;											// �����ֽڴ�С
UINT GameObject::mIndexCount;											// ������Ŀ	