
cbuffer WVPBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct VSin
{
	float3 position : POSITION;
	float2 tex : TEX;
	float4 color : COLOR;
};

struct VSout
{
	float4 position : Sv_Position;
	float2 tex : TEX;
	float4 color : COLOR;
};

VSout VS_main(VSin input)
{
	VSout output=(VSout)0;

	output.position			= float4( input.position, 1.0f );

	output.position			= mul( output.position, world );
	output.position			= mul( output.position, view );
	output.position			= mul( output.position, projection );  

	output.tex=input.tex;
	output.color=input.color;

	return output;
}

//================
// Pixel Shader ||
//================
Texture2D textest;
SamplerState samplerState;

//struct PSin
//{
	//float4 position : Sv_Position;
	//float2 tex : TEX;
	//float4 color : COLOR;
//};

float4 PS_main(VSout input): SV_Target
{
	float4 texColor;
	float4 finalColor;

	texColor=textest.Sample(samplerState, input.tex);

	finalColor=texColor* input.color;

	return finalColor;

}