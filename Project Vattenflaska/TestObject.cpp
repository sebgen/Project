#include "TestObject.h"

TestObject::TestObject()
	: DrawableObject()
{
	
}

TestObject::TestObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo )
{
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );
	rotationY = 0;
}

TestObject::~TestObject()
{}



//====================================================
// This function will exist in the Lever class and  ||
// be called upon when activated by the player.     ||
// A flag will be set as active and an animation    ||
// sequence will now occur in the objects Update    ||
//                   function.						||
//====================================================
//void TestObject::PullLever()
//{
//
//}
MeshInfo TestObject::getInfo()
{
	return m_meshInfo;
}
HRESULT TestObject::Update( float deltaTime, Camera* camera )
{
	HRESULT hr = S_OK;

	//rotationY += deltaTime * 0.1f;
	XMStoreFloat4x4( &m_CBmatrices.worldMatrix, XMMatrixRotationY(rotationY));

	return hr;
}

HRESULT TestObject::Draw( float deltaTime )
{
	HRESULT hr = S_OK;


	UpdateAndSetConstantBuffer();
	UpdateLightConstantBuffer();

	///TEST!
	UINT32 vertexSize	= sizeof( Vertex );
	UINT32 offset		= 0;
	ID3D11Buffer* buffersToSet[] = { m_vertexBuffer };
	m_deviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	m_deviceContext->PSSetSamplers( 0, 1, &m_samplerStateAnisotropic );  /// NY
	m_deviceContext->PSSetSamplers( 1, 1, &m_samplerStateLinear );  /// NY

		// Texture
	if( m_meshInfo.textureName.size() != 0 )
		m_deviceContext->PSSetShaderResources( 0, 1, &m_shaderResourceView );

	if( m_meshInfo.normalMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(1, 1, &m_normalmapRCV);

	if( m_meshInfo.specularMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(2, 1, &m_specularmapRCV);

	m_deviceContext->Draw( m_meshInfo.vertexCount, 0 );
	
	return hr;
}

void TestObject::Shutdown()
{
	// Release own objects first

	// Then release objects in base
	DrawableObject::Shutdown();
}