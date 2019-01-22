#include "Sprite3D.h"

ID3D11Device * Sprite3D::m_pD3DDevice = nullptr;
ID3D11DeviceContext * Sprite3D::m_pD3DDeviceContext = nullptr;
ID3DX11Effect * Sprite3D::m_pEffect = nullptr;
ID3DX11EffectTechnique * Sprite3D::m_pTechnique = nullptr;
ID3DX11EffectMatrixVariable* Sprite3D::m_pWorld = nullptr;
ID3D11InputLayout * Sprite3D::m_pInputLayout = nullptr;
ID3DX11EffectTechnique* Sprite3D::m_LightTechnique = nullptr;
ID3DX11EffectMatrixVariable* Sprite3D::m_pEffectWorldViewProj = nullptr;
ID3DX11EffectMatrixVariable* Sprite3D::m_pEffectWorld = nullptr;
ID3DX11EffectMatrixVariable* Sprite3D::m_pTexTransform = nullptr;
ID3DX11EffectMatrixVariable* Sprite3D::m_pEffectWorldInvTranspose = nullptr;
ID3DX11EffectVectorVariable* Sprite3D::m_pEffectEyePosW = nullptr;
LPD3D11EFFECTSHADERRESOURCEVARIABLE Sprite3D::m_pEffectShaderResourceVariable = nullptr;
ID3DX11EffectVariable* Sprite3D::m_pEffectMaterial = nullptr;

Sprite3D::Sprite3D() :m_pVertexBuff(nullptr)
{
	m_ModelWorld = XMMatrixIdentity();
	m_Location = m_InitialLocation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = m_InitialRotation = 0.0f;
	m_Scale = m_InitialScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	
	stride = sizeof(Vertex3D);
	offset = 0;
	m_Flags = 0;
}

Sprite3D::~Sprite3D()
{
	ClearCom();
}

void Sprite3D::Setup3D(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX, ID3D11InputLayout *pLayout)
{
	m_pD3DDevice = d3DDevice;
	m_pD3DDeviceContext = d3DDeviceContext;
	m_pEffect = fX;
	m_pInputLayout = pLayout;
	m_LightTechnique = m_pEffect->GetTechniqueByName("LightTech");
	m_pEffectWorldViewProj = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pEffectWorld = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pEffectWorldInvTranspose = m_pEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_pEffectEyePosW = m_pEffect->GetVariableByName("gEyePosW")->AsVector();
	m_pEffectMaterial = m_pEffect->GetVariableByName("gMaterial");
}

void Sprite3D::Release()
{
	
}

int Sprite3D::LoadMesh(int meshType, char * path, ID3D11Buffer ** pVertexBuff, std::vector<Vertex3D> &vertices)
{
	int v, vn, vt, num = 0;
	switch (meshType)
	{
	case OBJ:
	{
		std::vector<XMFLOAT3> vertex;
		std::vector<XMFLOAT2>	texcrood;
		std::vector<XMFLOAT3> normal;
		char type[2048];
		FILE * obj = fopen(path, "r");
		XMFLOAT3 f3;
		XMFLOAT2 f2;
		float maxX, minX, maxY, minY, maxZ, minZ;
		maxX = minX = maxY = minY = maxZ = minZ = 0;
		while (!feof(obj))				//遇到文件尾
		{
			fscanf_s(obj, "%s", type, 2048);	//过滤多余字符
			if (strcmp(type, "v") == 0)
			{
				fscanf_s(obj, "%f%f%f", &f3.x, &f3.y, &f3.z);
				vertex.push_back(f3);
			}
			else if (strcmp(type, "vt") == 0)
			{
				fscanf_s(obj, "%f%f", &f2.x, &f2.y);
				f2.y = 1 - f2.y;
				texcrood.push_back(f2);
			}
			else if (strcmp(type, "vn") == 0)
			{
				fscanf_s(obj, "%f%f%f", &f3.x, &f3.y, &f3.z);
				normal.push_back(f3);
			}
			else if (strcmp(type, "f") == 0)
			{
				fscanf_s(obj, "%d/%d/%d", &v, &vt, &vn);
				vertices.push_back(Vertex3D(vertex[v - 1], normal[vn - 1], texcrood[vt - 1]));

				fscanf_s(obj, "%d/%d/%d", &v, &vt, &vn);
				vertices.push_back(Vertex3D(vertex[v - 1], normal[vn - 1], texcrood[vt - 1]));

				fscanf_s(obj, "%d/%d/%d", &v, &vt, &vn);
				vertices.push_back(Vertex3D(vertex[v - 1], normal[vn - 1], texcrood[vt - 1]));

				num += 3;
			}
		}
		D3D11_BUFFER_DESC vertexBuffDesc;
		vertexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBuffDesc.ByteWidth = sizeof(Vertex3D) * vertices.size();
		vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBuffDesc.CPUAccessFlags = 0;
		vertexBuffDesc.MiscFlags = 0;
		vertexBuffDesc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = &vertices[0];
		HR(m_pD3DDevice->CreateBuffer(&vertexBuffDesc, &vertexInitData, pVertexBuff));
		break;
	}
	default:
		return false;
	}
	return num;
}

void Sprite3D::LoadTexture(char * path, ID3D11ShaderResourceView ** pTexture)
{
	D3DX11CreateShaderResourceViewFromFileA(m_pD3DDevice, path, nullptr, nullptr, pTexture, nullptr);
}

void Sprite3D::SetVertex(ID3D11Buffer * pVertex, int num)
{
	m_pVertexBuff = pVertex;
	m_NumIndex = num;
}

void Sprite3D::SetTexture(ID3D11ShaderResourceView * pTexture)
{
	m_ShaderResourceView = pTexture;
}

void Sprite3D::SetWorldProj(XMMATRIX &view, XMMATRIX & proj)
{
	m_WorldProj = m_ModelWorld*view*proj;
}

void Sprite3D::SetMatrial(Material & mat)
{
	m_Material.ambient = mat.ambient;
	m_Material.diffuse = mat.diffuse;
	m_Material.specular = mat.specular;
}

void Sprite3D::Move(XMFLOAT3 move)
{
	m_Location.x += move.x;
	m_Location.y += move.y;
	m_Location.z += move.z;
	CalculateWorldMatrix();
}

void Sprite3D::MoveTo(XMFLOAT3 moveto)
{
	m_Location.x = moveto.x;
	m_Location.y = moveto.y;
	m_Location.z = moveto.z;
	CalculateWorldMatrix();
}

void Sprite3D::Rotate(float radium)
{
	m_Rotation += radium;
	CalculateWorldMatrix();
}

void Sprite3D::Scale(XMFLOAT3 scale)
{
	m_Scale.x += scale.x;
	m_Scale.y += scale.y;
	m_Scale.z += scale.z;
	CalculateWorldMatrix();
}


void Sprite3D::SetReRolate(float rolate)
{
	m_InitialRotation = rolate;
}

void Sprite3D::ReSetSprite()
{
	m_Location = m_InitialLocation;
	m_Scale = m_InitialScale;
	m_Rotation = m_InitialRotation;
	CalculateWorldMatrix();
}

void Sprite3D::SetReLocation(XMFLOAT3 move)
{
	m_InitialLocation = move;
}

void Sprite3D::SetReLocation(float x, float y, float z)
{
	m_InitialLocation.x = x;
	m_InitialLocation.y = y;
	m_InitialLocation.z = z;
}

void Sprite3D::SetReScale(XMFLOAT3 scale)
{
	m_InitialScale = scale;
}

void Sprite3D::SetReScale(float x, float y, float z)
{
	m_InitialScale.x = x;
	m_InitialScale.y = y;
	m_InitialScale.z = z;
}

void Sprite3D::ClearCom()
{

}

void Sprite3D::Draw()
{
	m_pD3DDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pD3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex3D);
	UINT offset = 0;

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	D3DX11_TECHNIQUE_DESC techDesc;
	m_LightTechnique->GetDesc(&techDesc);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(m_ModelWorld);
	m_pD3DDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuff, &stride, &offset);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pEffectWorld->SetMatrix(reinterpret_cast<float*>(&m_ModelWorld));
		m_pEffectWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
		m_pEffectWorldViewProj->SetMatrix(reinterpret_cast<float*>(&m_WorldProj));
		m_pEffectMaterial->SetRawValue(&m_Material, 0, sizeof(m_Material));
		m_pEffectShaderResourceVariable = m_pEffect->GetVariableByName("Texture")->AsShaderResource();
		m_pEffectShaderResourceVariable->SetResource(m_ShaderResourceView);
		m_LightTechnique->GetPassByIndex(p)->Apply(0, m_pD3DDeviceContext);
		m_pD3DDeviceContext->Draw(m_NumIndex, 0);
	}
}

XMFLOAT3 Sprite3D::GetAveragePosition()
{
	return m_AveragePosition;
}

float Sprite3D::GetRadius()
{
	return m_Radius;
}

XMFLOAT3 Sprite3D::GetTranslationRelative()
{
	return XMFLOAT3(m_Location.x - m_InitialLocation.x, m_Location.y - m_InitialLocation.y, m_Location.z - m_InitialLocation.z);
}

XMFLOAT3 Sprite3D::GetTranslationAbsolute()
{
	return m_Location;
}

void Sprite3D::CalculateWorldMatrix()
{
	m_ModelWorld = XMMatrixRotationZ(m_Rotation)*XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z)*XMMatrixTranslation(m_Location.x, m_Location.y, m_Location.z);
}
