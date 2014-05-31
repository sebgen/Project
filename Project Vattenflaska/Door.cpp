#include "Door.h"

Door::Door()
	:DrawableObject()
{
	m_isOpening				= false;
	m_isOpen				= false;
	m_animationTimer		= 0.0f;
	m_moveUnits				= 0;
	m_sound					= nullptr;
	m_isOpeningSoundPlaying = false;
}

Door::Door( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo, EventManager* em )
{
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );

	m_isOpening		 = false;
	m_isOpen		 = false;
	m_animationTimer = 0.0f;
	m_moveUnits		 = 225;

	m_name = meshInfo.groupName.c_str();

	//Sound
	m_sound					= new Sound();
	m_sound->init();
	m_isOpeningSoundPlaying	= false;

	m_sound->addSoundEffect( L"doorIsOpening3SEC.wav", "isOpeningSound" );
	m_sound->addSoundEffect( L"doorIsOpen.wav", "isOpenSound" );

	m_em = em;

	// Open Door
	m_em->VAddListener(
		   std::bind(	&Door::OpenDoor,
						this,
						std::placeholders::_1),
						EvtData_Unlock_Door::sk_EventType);
}
MeshInfo Door::getInfo()
{
	return m_meshInfo;
}
Door::~Door()
{

}

void Door::OpenDoor( IEventDataPtr pEventData )
{
	if( EvtData_Unlock_Door::sk_EventType == pEventData->VGetEventType() || EvtData_Unlock_Maze_Door::sk_EventType == pEventData->VGetEventType() )
	{
		m_isOpening = true;
	}
}

void Door::RaiseDoor()
{
	// Increment Y-value for every vertex in mesh
	for (int i = 0; i < m_meshInfo.vertexCount; i++)
		m_meshInfo.vertices.at(i).position.y += 0.01f; 

	UpdateAndSetVertexBuffer();
}

void Door::CloseDoor( IEventDataPtr pEventData )
{

}

void Door::UpdateAndSetVertexBuffer()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_deviceContext->Map( m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if(SUCCEEDED(hr))
	{
		memcpy(mappedResource.pData, &m_meshInfo.vertices[0], m_meshInfo.vertexCount * sizeof( Vertex ) );
		m_deviceContext->Unmap( m_vertexBuffer, 0 );

		 //Vertex Buffer is set in Draw
	}

}

bool Door::IsOpen() const
{
	return m_isOpen;
}

const char* Door::GetName() const
{
	return m_name;
}

HRESULT Door::Update( float deltaTime, Camera* camera )
{	
	HRESULT hr = S_OK;

	if( m_isOpening && !m_isOpen )
	{
		// Play Opening sound
		if( !m_isOpeningSoundPlaying )
		{
			m_isOpeningSoundPlaying = true;
			m_sound->playSound( "isOpeningSound" );
		}
		

		m_animationTimer += deltaTime; // Add delta time to timer

		if( m_animationTimer >= 0.01f ) // It's time for next animation
		{
			m_animationTimer = 0.0f; // Reset timer
			RaiseDoor();			// Continue raising Door

			if( m_moveUnits <= 0 ) // If Door is fully open
			{
				m_moveUnits = 225;    // Reset move units
				m_isOpening	= false; // Door no longer moves
				m_isOpen	= true;  // Set Door to OPEN
				m_sound->playSound( "isOpenSound" );
				

				//===============================================
				// ADD EVENT HERE TO GET ACCESS TO NEW NAVMESH ||
				//===============================================
			}
			else
				m_moveUnits--; // Decreament units left to move
		}
	}

	return hr;
}

HRESULT Door::Draw( float deltaTime )
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

		// Texture
	if( m_meshInfo.textureName.size() != 0 )
		m_deviceContext->PSSetShaderResources( 0, 1, &m_shaderResourceView );

	if( m_meshInfo.normalMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(1, 1, &m_normalmapRCV);

	if( m_meshInfo.specularMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(2, 1, &m_specularmapRCV);;

	m_deviceContext->Draw( m_meshInfo.vertexCount, 0 );

	return hr;
}

void Door::Shutdown()
{
	DrawableObject::Shutdown();
}