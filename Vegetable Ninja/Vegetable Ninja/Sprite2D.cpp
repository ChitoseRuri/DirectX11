#include "Sprite2D.h"

ID3D11Device * Sprite2D::m_pD3DDevice = nullptr;
ID3D11DeviceContext * Sprite2D::m_pD3DDeviceContext = nullptr;
ID3DX11Effect * Sprite2D::m_pFX = nullptr;
ID3DX11EffectTechnique * Sprite2D::m_pTechnique = nullptr;
ID3DX11EffectMatrixVariable* Sprite2D::m_pWorld = nullptr;
ID3D11InputLayout * Sprite2D::m_pInputLayout = nullptr;
ID3D11BlendState* Sprite2D::m_pAlphaToCoverageBS = nullptr;

Sprite2D::Sprite2D() :m_pVertexBuff(nullptr), m_pIndexBuff(nullptr), m_pShaderResourceView(nullptr),
m_InitialLocation(XMFLOAT3(0, 0, 0)), m_Location(XMFLOAT3(0, 0, 0)),
m_InitialScale(XMFLOAT3(1.0f, 1.0f, 1.0f)), m_Scale(XMFLOAT3(1.0f, 1.0f, 1.0f)), m_InitialRotation(0.0f),
m_Rotation(0.0f), m_WorldMatrix(XMMatrixIdentity()), m_NumIndex(6)
{
}

Sprite2D::~Sprite2D()
{
	Release();
}

void Sprite2D::Setup2D(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX, ID3DX11EffectTechnique * pTechnique,
	ID3DX11EffectMatrixVariable* pWorld, ID3D11InputLayout * pInputLayout, ID3D11BlendState* pBlendState)
{
	m_pD3DDevice = d3DDevice;
	m_pD3DDeviceContext = d3DDeviceContext;
	m_pFX = fX;
	m_pTechnique = pTechnique;
	m_pWorld = pWorld;
	m_pInputLayout = pInputLayout;
	m_pAlphaToCoverageBS = pBlendState;
}

void Sprite2D::SetDevice(ID3D11Device * d3DDevice)
{
	m_pD3DDevice = d3DDevice;
}

void Sprite2D::SetContext(ID3D11DeviceContext * d3DDeviceContext)
{
	m_pD3DDeviceContext = d3DDeviceContext;
}

void Sprite2D::SetFX(ID3DX11Effect * fX)
{
	m_pFX = fX;
}

void Sprite2D::LoadPicture(char * path)
{
	ZeroMemory(m_Path, 256);
	strcpy_s(m_Path, path);
	D3DX11CreateShaderResourceViewFromFileA(m_pD3DDevice, m_Path, nullptr, nullptr, &m_pShaderResourceView, nullptr);
}

void Sprite2D::BuildGeometryBuffers(XMFLOAT2 leftUp, XMFLOAT2 rightDown, float deep)
{
	Vertex vertices[] =
	{ { XMFLOAT3(leftUp.x, leftUp.y, deep),  XMFLOAT2(0.0f,0.0f) },
	{ XMFLOAT3(rightDown.x, leftUp.y, deep),  XMFLOAT2(1.0f,0.0f) },
	{ XMFLOAT3(leftUp.x,rightDown.y, deep),  XMFLOAT2(0.0f,1.0f) },
	{ XMFLOAT3(rightDown.x, rightDown.y, deep),  XMFLOAT2(1.0f,1.0f) } };

	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBuffDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pD3DDevice->CreateBuffer(&vertexBuffDesc, &vinitData, &m_pVertexBuff));

	UINT indices[]
	{
		0,1,2,
		2,1,3
	};
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pD3DDevice->CreateBuffer(&ibd, &iinitData, &m_pIndexBuff));
}

void Sprite2D::BuildGeometryBuffers(ID3D11Buffer * buffer3D, unsigned num_vertex)
{
	m_pVertexBuff = buffer3D;
	std::vector<unsigned> indices;
	for (unsigned i = 0; i < num_vertex; i++)
	{
		indices.emplace_back(i);
	}
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * num_vertex;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_pD3DDevice->CreateBuffer(&ibd, &iinitData, &m_pIndexBuff));
	m_NumIndex = num_vertex;
}

void Sprite2D::BuildFX()
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"FX/color.fx", 0, 0, 0, "fx_5_0", shaderFlags,
		0, 0, &compiledShader, &compilationMsgs, 0);

	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		ReleaseCOM(compilationMsgs);
	}

	// Even if there are no compilationMsgs, check to make sure there were no other errors.
	if (FAILED(hr))
	{
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, m_pD3DDevice, &m_pFX));

	ReleaseCOM(compiledShader);

	m_pTechnique = m_pFX->GetTechniqueByName("TexTech");
	
	m_pWorld = m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Sprite2D::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pD3DDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));
}

void Sprite2D::BuidTexture()
{
	m_pFXEffectShaderSourceVariable = m_pFX->GetVariableByName("Texture")->AsShaderResource();
	m_pFXEffectShaderSourceVariable->SetResource(m_pShaderResourceView);
}

void Sprite2D::UpdateScene(float dt)
{
}

void Sprite2D::DrawScene()
{
	m_pD3DDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	BuidTexture();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pD3DDeviceContext->OMSetBlendState(m_pAlphaToCoverageBS, blendFactor, 0xffffffff);
	m_pD3DDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuff, &stride, &offset);
	m_pD3DDeviceContext->IASetIndexBuffer(m_pIndexBuff, DXGI_FORMAT_R32_UINT, 0);

	m_pWorld->SetMatrix(reinterpret_cast<float*>(&m_WorldMatrix));

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, m_pD3DDeviceContext);

		m_pD3DDeviceContext->DrawIndexed(m_NumIndex, 0, 0);
	}

}

void Sprite2D::Release()
{
	ReleaseCOM(m_pVertexBuff);
	ReleaseCOM(m_pIndexBuff);
	ReleaseCOM(m_pShaderResourceView);
}

void Sprite2D::ImgRotate(float angle)
{
	m_Rotation += angle;
	m_WorldMatrix = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}

void Sprite2D::ImgMove(float x, float y, float depth)
{
	m_Location.x += x;
	m_Location.y += y;
	m_Location.z += depth;
	m_WorldMatrix = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}

void Sprite2D::ImgMove(float x, float y)
{
	m_Location.x += x;
	m_Location.y += y;
	m_WorldMatrix = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}

void Sprite2D::ImgMoveTo(float x, float y, float depth)
{
	m_Location.x = x - m_InitialLocation.x;
	m_Location.y = y - m_InitialLocation.y;
	m_Location.z = depth - m_InitialLocation.z;
	m_WorldMatrix = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}

void Sprite2D::ImgMoveTo(float x, float y)
{
	m_Location.x = x - m_InitialLocation.x;
	m_Location.y = y - m_InitialLocation.y;
	m_WorldMatrix = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}

void Sprite2D::ImgScale(float x, float y, float z)
{
	m_Scale.x += x;
	m_Scale.y += y;
	m_Scale.z += z;
	m_WorldMatrix = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}

void Sprite2D::SetReLocation(XMFLOAT3 move)
{
	m_InitialLocation = move;
}

void Sprite2D::SetReLocation(float x, float y, float z)
{
	m_InitialLocation.x = x;
	m_InitialLocation.y = y;
	m_InitialLocation.z = z;
}

void Sprite2D::SetReScale(XMFLOAT3 scale)
{
	m_InitialScale = scale;
}

void Sprite2D::SetReScale(float x, float y, float z)
{
	m_InitialScale.x = x;
	m_InitialScale.y = y;
	m_InitialScale.z = z;
}

void Sprite2D::SetReRolate(float rolate)
{
	m_InitialRotation = rolate;
}

void Sprite2D::ReSetLocation()
{
	m_Rotation = m_InitialRotation;
	m_Location = m_InitialLocation;
	m_Scale = m_InitialScale;
}
