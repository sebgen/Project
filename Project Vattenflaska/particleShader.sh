
cbuffer WVPBuffer
{
	float4x4 view;
	float4x4 projection;
};

struct VSin
{
	float4 position : POSITION;
	float2 tex : TEX;

};

struct VSout
{
	float4 position : Sv_Position;
	float2 tex : TEX;
};

VSout VS_main(VSin input)
{
	VSout output=(VSout)0;

	//output.position			= float4( input.position, 1.0f );
	input.position.w = 1.0f;

	output.position			= mul( input.position, view );
	output.position			= mul( output.position, projection );  

	output.tex=input.tex;
	//output.color=input.color;

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
	

	texColor=textest.Sample(samplerState, input.tex);


	
	return texColor;

}