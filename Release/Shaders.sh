//----------------------------
// Datatypes used in shaders |
//----------------------------
cbuffer WVPBuffer
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

struct VSIn
{
	float3 position : POSITION;
	float3 normal   : NORMAL;
	float2 texCoord : TEX;
};

struct VSOut
{
	float4 position			: SV_POSITION;
	float4 worldPosition	: WORLD_POSITION;
	float3 normal			: NORMAL;
	float2 texCoord			: TEX;
};


//=================
// Vertex Shader ||
//=================
VSOut VS_main( VSIn input )
{
	VSOut output = (VSOut)0;

	output.position			= float4( input.position, 1.0f );

	output.position			= mul( output.position, world );
	output.worldPosition	= output.position;
	output.position			= mul( output.position, view );
	output.position			= mul( output.position, projection );  

	output.normal			= mul( input.normal, world );
	output.texCoord			= input.texCoord;
	
	return output;

}


//-----------------------------------------
// Variables affiliated with Pixel shader |
//-----------------------------------------
Texture2D tex;
SamplerState samplerState;

struct Light
{
	float4 ambient;
	float4 diffuse;
	float3 position;
	float  range;
	float3 attenuation;

};

cbuffer LightBuffer
{
	Light light;
};

//================
// Pixel Shader ||
//================
float4 PS_main( VSOut input ) : SV_Target
{	
	input.normal = normalize( input.normal );

	float4 diffuse = tex.Sample( samplerState, input.texCoord );

	float3 finalColor = float3( 0.0f, 0.0f, 0.0f );

	// Create the vector between light position and pixels position
	float3 lightToPixelVec = light.position - input.worldPosition;

	// Find the distance between the light position and pixel position
	float d = length( lightToPixelVec );

	// Create the ambient light
	float3 finalAmbient = diffuse * light.ambient;
	finalAmbient *= 0.1f;

	// If pixel is too far, return pixel color with ambient light
	if( d > light.range )
		return float4( finalAmbient, diffuse.a );

	// Turn lightPixelVec into a unit length vector describing
	// the pixels direction from the lights position
	lightToPixelVec /= d;

	// Calculate how much light the pixel gets by the angle
	// in which the light strikes the pixels surface
	float howMuchLight = dot( lightToPixelVec, input.normal );

	// If light is striking the front side of the pixel
	if( howMuchLight > 0.0f )
	{
		// Add light to the finalColor of the pixel
		finalColor += howMuchLight * diffuse * light.diffuse;

		// Calculate Light's Falloff factor
		finalColor /= light.attenuation[0] + ( light.attenuation[1] * d )
				    + ( light.attenuation[2] * ( d * d ) );
	}

	// Make sure the values are between 1 and 0, and add the ambient
	finalColor = saturate( finalColor + finalAmbient );

	// Return Final Color
	return float4( finalColor, diffuse.a );

	//return float4( tex.Sample( samplerState, input.texCoord ) );
}