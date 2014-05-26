#include "Lever.h"

Lever::Lever()
	:DrawableObject()
{
}

Lever::Lever( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo, EventManager* em  )
{
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );

	// Lua / Event
	m_isOn = false;
	m_name = meshInfo.groupName;
	m_em = em;

	// Send event: EvtData_Lever_Created
	IEventDataPtr e(GCC_NEW EvtData_Lever_Created( this ) );
	m_em->VQueueEvent( e );
}

Lever::~Lever()
{}
MeshInfo Lever::getInfo()
{
	return m_meshInfo;
}


HRESULT Lever::Update( float deltaTime, Camera* camera )
{
	HRESULT hr = S_OK;

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

// Lua / Event
void Lever::PullLever()
{
	if( !m_isOn )
		m_isOn = true;

	// DEBUG =======================================
	IEventDataPtr d(GCC_NEW EvtData_Unlock_Door() );
	m_em->VQueueEvent( d );
	//==============================================
	//IEventDataPtr e(GCC_NEW EvtData_Lever_Pull( this ) );
	//m_em->VQueueEvent( e );
}

bool Lever::IsOn() const 
{
	return m_isOn;
}

std::string Lever::GetName() const
{
	return m_name;
}

void Lever::ResetLever()
{
	m_isOn = false;
}