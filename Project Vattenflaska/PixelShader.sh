//--------------------------------------------------------------------------------------
// Tri.ps
// Direct3D 11 Shader Model 4.0 Demo
// Copyright (c) Stefan Petersson, 2011
//--------------------------------------------------------------------------------------

struct PSIn
{
	float4 PosH  : SV_Position;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 TexCoord : TEX;
	float3 TangentW : TANGENT;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samAnisotropic : register(s0);
SamplerState samLinear : register(s1);
 

struct Pointlight
{
	float4 Diffuse;
	float4 Position_Range;
	float Intensity;
	float3 pad;
};

cbuffer cbPerFrame : register(b0)
{
	Pointlight plights[50];
	float4 gEyePosW;
	float nroflights;
};

cbuffer cbPerMesh : register(b2)
{
	float useNormalMap;
	float useSpecularMap;
};

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	//Uncompress each component from 0,1 to -1,1
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	//Build orthonormal basis
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)*N);
	float3 B = cross(N,T);

	float3x3 TBN = float3x3(T,B,N);

	//Transform from tangent space to world space
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

void ComputePointLight(Pointlight p, float3 pos, float3 normal, float2 tex, float3 toEye,
						out float4 ambient, out float4 diffuse, out float4 spec)
{
	//Initialize outputs
	ambient = float4(0.0f, 0.0f, 0.0f,0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 lSpec = float4(0.1f, 0.1f, 0.1f, 1.0f);

	// The vector from the surface to the light
	float3 lightVec = p.Position_Range.xyz - pos;

	// The distance from surface to light
	float d = length(lightVec);

	// Normalize the light vector
	lightVec /= d;

	// Range test
	if(d > 15.0f)
		return;
	
	//Check if plane is towards the light
	float3 lightTest = (pos + normal) - pos;
	lightTest = normalize(lightTest);
	float test = dot(lightVec, lightTest);

	//if false, cancle further calc
	if (test < 0)
		return;

	//Ambient term
	diffuse = p.Diffuse * p.Position_Range.w;
	ambient = float4(0.01f, 0.01f, 0.01f, 1.0f);

	float diffuseFactor = dot(lightVec, normal);

	//flatten to avoid dynamic branching
	[flatten]
	if(diffuseFactor > 0.0f)
	{
		if(useSpecularMap == 1.0f)
		{
			float4 specularMap = gSpecularMap.Sample(samLinear, tex) * 0.1f;
			float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0f), specularMap.w);
			spec = specFactor * specularMap * lSpec;
		}
		else
			spec = lSpec * 0.1f;

		diffuse = diffuseFactor * p.Diffuse;
	}

	//Attenuate
	float att = (1.0f / dot(float3(0.1f, 0.1f, 0.1f), float3(1.0f, d, d*d))) * p.Position_Range.w;
	
	diffuse *= att;
	//ambient *= att;
	spec *= att;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PS_main(PSIn input) : SV_Target
{		
	//Calculate distance to eye;
	float3 toEye = gEyePosW.xyz - input.PosW;
	float3 distToEye = length(toEye);
	float dl = length(toEye);
	toEye /= distToEye;
	
	//Sampling texture
	float4 texColor = float4(0,0,0,0);	
	texColor = gDiffuseMap.Sample(samAnisotropic, input.TexCoord);

		
	//If range to far, no calculations needed 
	//if(dl > 35.0f || dl < 0.0f)
		//return texColor * float4(0.03f, 0.03f, 0.03f, 1.0f);

	//If no normalmap available
	float3 bumpedNormalW = input.NormalW;
	if(useNormalMap == 1)
	{
		//NormalMap available
		float3 normalMapSample = gNormalMap.Sample(samLinear, input.TexCoord).rgb;
		bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, input.NormalW, input.TangentW);
	}

	//Lightning
	float4 litColor = texColor;
	float4 ambient = float4(0.03f,0.03f,0.03f,1.0f);
	//float4 ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
	float4 diffuse = float4(0,0,0,0);
	float4 spec = float4(0,0,0,0);
	float4 A,D,S;

	for(int i = 0; i < 4; i++)
	{
		float3 lightToEye = plights[i].Position_Range.xyz - gEyePosW.xyz;
		float l = length(lightToEye);

		if(l < 30.0f)
		{
			ComputePointLight(plights[i], input.PosW, bumpedNormalW, input.TexCoord, toEye, A, D, S);
		
			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	litColor = texColor * (diffuse + ambient) + spec;
	return litColor;
}