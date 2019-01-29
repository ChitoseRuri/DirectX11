#include "GameApp.h"

//静态共享成员
ComPtr<ID3D11Buffer> GameObject::mVertexBuffer;							// 顶点缓冲区
ComPtr<ID3D11Buffer> GameObject::mIndexBuffer;							// 索引缓冲区
std::vector<ComPtr<ID3D11ShaderResourceView>> GameObject::mTextures;	// 纹理
UINT GameObject::mVertexStride;											// 顶点字节大小
UINT GameObject::mIndexCount;											// 索引数目	