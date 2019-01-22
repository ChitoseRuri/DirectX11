#ifndef LIGHT_H
#define LIGHT_H

#include <Windows.h>
#include <xnamath.h>
#include "d3dUtil.h"



struct DirectionalLight				//ƽ�й�ṹ��
{
	DirectionalLight() { ZeroMemory(this, sizeof(*this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 Direction;
	int State; // Pad the last float so we can set an array of lights if we wanted.
};

struct PointLight						 //���Դ�ṹ��
{
	PointLight() { ZeroMemory(this, sizeof(*this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;
	// Packed into 4D vector: (A0, A1, A2, Pad)
	XMFLOAT3 Att;
	int State; // Pad the last float so we can set an array of lights if we wanted.
};

struct SpotLight						//�۹�ƽṹ��
{
	SpotLight() { ZeroMemory(this, sizeof(*this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;

	// Packed into 4D vector: (Position, Range)
	XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	XMFLOAT3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	XMFLOAT3 Att;
	int State; // Pad the last float so we can set an array of lights if we wanted.
};

//struct Material										//������ʽṹ��
//{
//	Material() { ZeroMemory(this, sizeof(*this)); }
//
//	XMFLOAT4 ambient;
//	XMFLOAT4 diffuse;
//	XMFLOAT4 specular; // w = SpecPower
//	XMFLOAT4 Reflect;
//};

enum  LightType { DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT };
enum LightState { UNUSED, ON, OFF };

class LightControl
{
public:

	void SetFX(ID3DX11Effect * fx);
	static LightControl * GetLightContal();
	static void ReleaseLight(LightControl * pLights);
	//���õƹ�
	template <typename LightType>
	bool SetLight(char * name, DirectionalLight light);
	template <typename LightType>
	bool SetLight(char * name, PointLight light);
	template <typename LightType>
	bool SetLight(char * name, SpotLight light);


	bool OnLight(char * name, int lightType);
	bool OffLight(char * name, int lightType);
	bool DeleteLight(char * name, int lightType);

	void SetValue();
	static LightControl * m_pLightControl;
	static ID3DX11Effect * m_pLightEffect;
	
private: 
	LightControl();
	~LightControl();
	int FindIndex(char * name, int type);

	DirectionalLight m_DirectionalLights[16];
	PointLight m_PointLights[16];
	SpotLight m_SpotLights[16];
	char * m_LightName[3][16];
	int m_LightState[3][16];


	ID3DX11EffectVariable* m_FXDirLight;
	ID3DX11EffectVariable* m_FXPointLight;
	ID3DX11EffectVariable* m_FXSpotLight;
	ID3DX11EffectVariable* m_FXMaterial;
};

#endif // !LIGHT_H


