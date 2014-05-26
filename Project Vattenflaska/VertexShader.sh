//--------------------------------------------------------------------------------------
// DrawTri.vs
// Direct3D 11 Shader Model 4.0 Demo
// Copyright (c) Stefan Petersson, 2011
//--------------------------------------------------------------------------------------

struct VSIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
	float3 Tangent : TANGENT;
};

struct PSIn
{
	float4 PosH  : SV_Position;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 TexCoord : TEX;
	float3 TangentW : TANGENT;
};

cbuffer CB_PER_FRAME : register(b0)
{
	matrix view;
	matrix proj;
	matrix viewProj;
}

cbuffer CB_PER_OBJECT : register(b1)
{
	matrix world;
}


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn VS_main(VSIn input)
{
	PSIn output = (PSIn)0;

	//output.Normal = input.Normal;
	output.TexCoord = input.TexCoord;	
	// float4(input.Pos, 1.0f)
	output.PosH = mul(float4(input.Pos, 1.0f), mul(world, viewProj));
	output.PosW = mul(float4(input.Pos, 1.0f), world).xyz;
	output.NormalW = mul(input.Normal, (float3x3)world);
	output.TangentW = mul(input.Tangent, (float3x3) world);

	output.NormalW = normalize(output.NormalW);
		
	return output;
}