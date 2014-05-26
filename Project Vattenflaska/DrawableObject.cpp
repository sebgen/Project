/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
-------------------------------------------------------------------------
  History:
  2014-04-25 
  Created by Simon Johanson

*************************************************************************/

#include "DrawableObject.h"

// Static object counter that increments every
// time an object is created. After each increment
// the new object will be given the counters new 
// value as an unique object id
UINT32 DrawableObject::s_nrOfObjects = 0;

HRESULT DrawableObject::CreateTextureAndSamplerState()  /// NY
{
	// Convert std:string to std::wstring
	std::wstring texture( m_meshInfo.textureName.begin(), m_meshInfo.textureName.end() );

	// Read texture
	HRESULT hr = CreateDDSTextureFromFile( m_device , texture.c_str(), (ID3D11Resource**)&m_texture, nullptr );
	if( FAILED( hr ) )
		return hr;

	//==============================
	// Create Anisotropic Sampler ||
	//==============================
	D3D11_SAMPLER_DESC samAniDesc;
	memset( &samAniDesc, 0, sizeof( samAniDesc ) );
	samAniDesc.Filter			= D3D11_FILTER_ANISOTROPIC;
	samAniDesc.AddressU			= D3D11_TEXTURE_ADDRESS_WRAP;
	samAniDesc.AddressV			= D3D11_TEXTURE_ADDRESS_WRAP;
	samAniDesc.AddressW			= D3D11_TEXTURE_ADDRESS_WRAP;
	samAniDesc.MipLODBias		= 0.0f;
	samAniDesc.MaxAnisotropy	= 4;
	samAniDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;

	if( FAILED( hr = m_device->CreateSamplerState( &samAniDesc, &m_samplerStateAnisotropic ) ) )
		return hr;

	//==============================
	// Create Anisotropic Sampler ||
	//==============================
	D3D11_SAMPLER_DESC samLinDesc;
	memset( &samLinDesc, 0, sizeof( samLinDesc ) );
	samLinDesc.Filter		= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samLinDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	samLinDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	samLinDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	samLinDesc.MipLODBias		= 0.0f;
	samLinDesc.MaxAnisotropy	= 1;
	samLinDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	
	if( FAILED( hr = m_device->CreateSamplerState( &samLinDesc, &m_samplerStateLinear ) ) )
		return hr;


	hr = m_device->CreateShaderResourceView( m_texture, nullptr, &m_shaderResourceView );
	return hr;
}

HRESULT DrawableObject::CreateNormalMap()
{
	// Convert std:string to std::wstring
	std::wstring normalMap( m_meshInfo.normalMapName.begin(), m_meshInfo.normalMapName.end() );

	// Read texture
	HRESULT hr = CreateDDSTextureFromFile( m_device , normalMap.c_str(), (ID3D11Resource**)&m_texture, nullptr );
	if( FAILED( hr ) )
		return hr;

	return hr = m_device->CreateShaderResourceView( m_texture, nullptr, &m_normalmapRCV );
}

HRESULT DrawableObject::CreateSpecularMap()
{
	// Convert std:string to std::wstring
	std::wstring specularMap( m_meshInfo.specularMapName.begin(), m_meshInfo.specularMapName.end() );

	// Read texture
	HRESULT hr = CreateDDSTextureFromFile( m_device , specularMap.c_str(), (ID3D11Resource**)&m_texture, nullptr );
	if( FAILED( hr ) )
		return hr;

	return hr = m_device->CreateShaderResourceView( m_texture, nullptr, &m_specularmapRCV );
}

void DrawableObject::UpdateAndSetConstantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_deviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );


	if(SUCCEEDED(hr))
	{
		memcpy(mappedResource.pData, &m_CBmatrices, sizeof(m_CBmatrices));
		m_deviceContext->Unmap( m_constantBuffer, 0 );

		m_deviceContext->VSSetConstantBuffers(1, 1, &m_constantBuffer);
	}
}

void DrawableObject::UpdateLightConstantBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_deviceContext->Map(m_lightInfoBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if(SUCCEEDED(hr))
	{
		m_CBLightInfo.normalMapEnable = m_useNormalMap;
		m_CBLightInfo.specularMapEnable = m_useSpecularMap;

		memcpy(mappedResource.pData, &m_CBLightInfo, sizeof(m_CBLightInfo));
		m_deviceContext->Unmap( m_lightInfoBuffer, 0 );

		m_deviceContext->PSSetConstantBuffers(2, 1, &m_lightInfoBuffer);
	}
}

DrawableObject::DrawableObject()
{
	// Increment object count
	s_nrOfObjects++;

	m_device				= nullptr;
	m_deviceContext			= nullptr;
	m_objectID				= s_nrOfObjects;
	m_vertexBuffer			= nullptr;
	m_constantBuffer		= nullptr;


	//Texture	
	m_texture				  = nullptr;
	m_samplerStateAnisotropic = nullptr;
	m_samplerStateLinear	  = nullptr;
	m_shaderResourceView	  = nullptr;
	m_normalmapRCV			  = nullptr;
	m_specularmapRCV		  = nullptr;

	// Light
	m_useNormalMap			= 0.0f;
	m_useSpecularMap		= 0.0f;
	XMStoreFloat4x4( &m_CBmatrices.worldMatrix, XMMatrixIdentity());
}

DrawableObject::DrawableObject( const DrawableObject& rhs )
{
	m_vertexBuffer = rhs.m_vertexBuffer;
	m_constantBuffer = rhs.m_constantBuffer;
}

DrawableObject::~DrawableObject()
{

}

ID3D11Buffer* DrawableObject::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

std::string DrawableObject::GetGroupName() const
{
	return m_meshInfo.groupName;
}

float DrawableObject::GetUseNormalMap() const
{
	return m_useNormalMap;
}

float DrawableObject::GetUseSpecularMap() const
{
	return m_useSpecularMap;
}

void DrawableObject::SetObjectData( MeshInfo meshInfo )
{
	//=================
	// Set Mesh Info ||
	//=================
	m_meshInfo.groupName		= meshInfo.groupName;
	m_meshInfo.normalMapName	= meshInfo.normalMapName;
	m_meshInfo.specularMapName	= meshInfo.specularMapName;
	m_meshInfo.textureName		= meshInfo.textureName;
	m_meshInfo.vertexCount		= meshInfo.vertexCount;
	m_meshInfo.vertices			= meshInfo.vertices;

	//========================
	// Create Vertex Buffer ||
	//========================
	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth			= sizeof(Vertex) * m_meshInfo.vertexCount;
	vbd.StructureByteStride = sizeof(Vertex);
	vbd.Usage				= D3D11_USAGE_DEFAULT;
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= 0;
	vbd.MiscFlags			= 0;

	if( m_meshInfo.groupName == "door_gateShape" )
	{
		vbd.Usage			= D3D11_USAGE_DYNAMIC;
		vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	}
	
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.SysMemPitch		= 0;
	vinitData.SysMemSlicePitch	= 0;
	vinitData.pSysMem			= &m_meshInfo.vertices[0];
	HRESULT hr = m_device->CreateBuffer(&vbd, &vinitData, &m_vertexBuffer);

	//=========================
	// Create Object Texture ||
	//=========================
	CreateTextureAndSamplerState();
	CreateNormalMap();
	CreateSpecularMap();

	if( m_meshInfo.normalMapName.size() != 0 )
		m_useNormalMap = 1.0f;

	if( m_meshInfo.specularMapName.size() != 0 )
		m_useSpecularMap = 1.0f;

	//========================
	// Create Matrix Buffer ||
	//========================

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof( WVP_PerObjectBuffer );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;
	
	if(FAILED(hr = m_device->CreateBuffer( &cbDesc, NULL, &m_constantBuffer)))
	{
		OutputDebugString("failed to create constant buffer in DrawableObject\n");
	}

	//============================
	// Create Light Info Buffer ||
	//============================  /// NY

	// Fill in a buffer description.
	D3D11_BUFFER_DESC cbLightDesc;
	cbLightDesc.ByteWidth = sizeof( WVP_PerObjectLightBuffer );
	cbLightDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbLightDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbLightDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbLightDesc.MiscFlags = 0;
	cbLightDesc.StructureByteStride = 0;
	
	if(FAILED(hr = m_device->CreateBuffer( &cbLightDesc, NULL, &m_lightInfoBuffer)))
	{
		OutputDebugString("failed to create light buffer in DrawableObject\n");

	}
	
	
}

bool DrawableObject::operator==( const DrawableObject& rhs )
{
	return m_objectID == rhs.m_objectID;
}

//=========================================
// Pure virtual functions Update & Draw  ||
//  is implemented in derived classes    ||
//=========================================

void DrawableObject::Shutdown()
{
	SAFE_RELEASE( m_vertexBuffer );
	SAFE_RELEASE( m_constantBuffer );

	//Texture
	SAFE_RELEASE( m_texture );
	SAFE_RELEASE( m_samplerStateAnisotropic );
	SAFE_RELEASE( m_samplerStateLinear );
	SAFE_RELEASE( m_shaderResourceView );
	SAFE_RELEASE( m_normalmapRCV );
	SAFE_RELEASE( m_specularmapRCV );
}

//Debug
std::string DrawableObject::DebugName()
{
	return m_meshInfo.groupName;
}