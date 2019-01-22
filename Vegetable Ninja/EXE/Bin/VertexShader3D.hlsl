#include "LightHelper.hlsl"

Texture2D Texture;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;
    AddressU = WRAP;
    AddressV = WRAP;
};

cbuffer cbPerFrame
{
    DirectionalLight gDirLight[16];
    PointLight gPointLight[16];
    SpotLight gSpotLight[16];
    float3 gEyePosW;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
    float4x4 gWorldViewProj;
    Material gMaterial;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;

	// Transform to world space space.
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3) gWorldInvTranspose);

	// Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Tex = vin.Tex;
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

    float3 toEyeW = normalize(gEyePosW - pin.PosW);

	// Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);



	//Œ∆¿Ì
    float4 texColor = float4(1, 1, 1, 1);
    texColor = Texture.Sample(samAnisotropic, pin.Tex);
    //clip(texColor.a - .01f);
    float4 litColor = texColor;

	// Sum the light contribution from each light source.
    float4 A, D, S;
	[unroll]
    for (int i = 0; i < 16; i++)
    {
        if (gDirLight[i].State == 1)
        {
            ComputeDirectionalLight(gMaterial, gDirLight[i], pin.NormalW, toEyeW, A, D, S);
            ambient += A;
            diffuse += D;
            spec += S;
        }

        if (gPointLight[i].State == 1)
        {
            ComputePointLight(gMaterial, gPointLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
            ambient += A;
            diffuse += D;
            spec += S;
        }

        if (gSpotLight[i].State == 1)
        {
            ComputeSpotLight(gMaterial, gSpotLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }




    litColor = texColor * (ambient + diffuse) + spec;
	
// Common to take alpha from diffuse material.
    litColor.a = gMaterial.diffuse.a * texColor.a;
	
    return litColor;
}

technique11 LightTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}