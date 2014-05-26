#include "Wheel.h"

Wheel::Wheel()
	: DrawableObject()
{
	m_isRotating	= false;
	m_rotationTimer = 0.0f;
}

Wheel::Wheel( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo )
{
	m_isRotating	= false;
	m_rotationTimer = 0.0f;
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );
}

Wheel::~Wheel()
{}

void Wheel::RotateWheel()
{

}


HRESULT Wheel::Update( float deltaTime, Camera* camera )
{
	HRESULT hr = S_OK;

	//// Lever is in UPPER state
	//if ( m_isRotating && !m_isActive )
	//{
	//	m_rotationTimer += deltaTime;

	//	// Rotation is finished
	//	if ( m_rotationTimer == 1.0f )
	//	{
	//		m_rotationTimer = 0.0f;
	//		m_isActive		= true;
	//	}
	//	else
	//		RotateWheel( 1.0f );
	//}
	//// Lever is in LOWER state
	//else if( m_isRotating && m_isActive )
	//{
	//	m_rotationTimer += deltaTime;

	//	// Rotation is finished
	//	if ( m_rotationTimer == 1.0f )
	//	{
	//		m_rotationTimer = 0.0f;
	//		m_isActive		= false;
	//	}
	//	else
	//		RotateWheel( 1.0f );
	//}

	return hr;
}

HRESULT Wheel::Draw( float deltaTime )
{
	HRESULT hr = S_OK;

	///TEST!
	UINT32 vertexSize	= sizeof( Vertex );
	UINT32 offset		= 0;
	ID3D11Buffer* buffersToSet[] = { m_vertexBuffer };
	m_deviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );
	
	// Texture
	m_deviceContext->PSSetShaderResources( 0, 1, &m_shaderResourceView );

	m_deviceContext->Draw( m_meshInfo.vertexCount, 0 );
	
	return hr;
}

void Wheel::Shutdown()
{
	// Release own objects first!

	DrawableObject::Shutdown();
}