#include "Lever.h"

Lever::Lever()
	:DrawableObject()
{
	/*m_isActive		= false;
	m_isRotating	= false;*/

}

Lever::Lever( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo )
{
	/*m_isActive		= false;
	m_isRotating	= false;*/
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );
}

Lever::~Lever()
{}

//void Lever::RotateLever( float angle )
//{
//	m_leverWorld = XMMatrixRotationZ( angle );
//}
//
//bool Lever::IsActive() const
//{
//	return m_isActive;
//}
//
//bool Lever::IsRotating( bool state )
//{
//	m_isRotating = state;
//	return m_isRotating;
//}

HRESULT Lever::Update( float deltaTime, Camera* camera )
{
	HRESULT hr = S_OK;


	//// Lever is in UPPER state
	//if ( m_isRotating && !m_isActive )
	//{
	//	m_rotationTimer += deltaTime;

	//	// Rotation is finished
	//	if ( m_rotationTimer >= 1.0f )
	//	{
	//		m_rotationTimer = 0.0f;
	//		m_isActive		= true;
	//		m_isRotating	= false;
	//	}
	//	else
	//		RotateLever( 1.0f );
	//}
	//// Lever is in LOWER state
	//else if( m_isRotating && m_isActive )
	//{
	//	m_rotationTimer += deltaTime;

	//	// Rotation is finished
	//	if ( m_rotationTimer >= 1.0f )
	//	{
	//		m_rotationTimer = 0.0f;
	//		m_isActive		= false;
	//		m_isRotating	= false;
	//	}
	//	else
	//		RotateLever( -1.0f );
	//}

	return hr;
}

HRESULT Lever::Draw( float deltaTime )
{
	HRESULT hr = S_OK;

	UpdateAndSetConstantBuffer();
	UpdateLightConstantBuffer();

	UINT32 vertexSize	= sizeof( Vertex );
	UINT32 offset		= 0;
	ID3D11Buffer* buffersToSet[] = { m_vertexBuffer };
	m_deviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	//OutputDebugString("sett constatbuffer to lever\n");

	m_deviceContext->PSSetSamplers( 0, 1, &m_samplerStateAnisotropic );  /// NY
	m_deviceContext->PSSetSamplers( 1, 1, &m_samplerStateLinear );  /// NY

	if( m_meshInfo.textureName.size() != 0 )
		m_deviceContext->PSSetShaderResources( 0, 1, &m_shaderResourceView );

	if( m_meshInfo.normalMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(1, 1, &m_normalmapRCV);

	if( m_meshInfo.specularMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(2, 1, &m_specularmapRCV);

	m_deviceContext->Draw( m_meshInfo.vertexCount, 0 );
	
	return hr;
}

void Lever::Shutdown()
{
	DrawableObject::Shutdown();
}