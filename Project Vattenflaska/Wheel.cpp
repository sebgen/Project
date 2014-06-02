#include "Wheel.h"

Wheel::Wheel()
	: DrawableObject()
{
	m_isRotating	= false;
	m_rotationTimer = 0.0f;
}

Wheel::Wheel( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo, EventManager* em  )
{
	


	m_isRotating	= false;
	m_rotationTimer = 0.0f;
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );

	m_name		= meshInfo.groupName;
	m_em		= em;
	m_value		= 1;
	m_isOn		= false;
	m_isMusicPanelOn = false;

	m_sound = new Sound();
	m_sound->init();
	m_sound->addSoundEffect( L"lever_pull.wav", "rotateSound" );
	m_sound->addSoundEffect( L"startMusicPlayer.wav", "startMusicPlayer" );

	// Blow sounds
	m_sound->addSoundEffect( L"cave_blowSound1.wav", "blowSound1" );
	m_sound->addSoundEffect( L"cave_blowSound2.wav", "blowSound2" );
	m_sound->addSoundEffect( L"cave_blowSound3.wav", "blowSound3" );
	m_sound->addSoundEffect( L"cave_blowSound4.wav", "blowSound4" );


	if( m_name == "wheel1Shape" ||
		m_name == "wheel2Shape" ||
		m_name == "wheel3Shape" ||
		m_name == "wheel4Shape" )
					m_wheelType = MusicWheel;

	else if(    m_name == "wheel5Shape" ||
				m_name == "wheel6Shape" ||
				m_name == "wheel7Shape" ||
				m_name == "wheel8Shape" )
					m_wheelType = BoilerWheel;

	else if(    m_name == "firstMusicWheelShape" ||
				m_name == "lastMusicWheelShape" )
					m_wheelType = ButtonWheel;

	// Start Music Panel
	m_em->VAddListener(
		   std::bind(	&Wheel::StartMusicPlayer,
						this,
						std::placeholders::_1),
						EvtData_Start_MusicPanel::sk_EventType);

	// Send event: EvtData_Wheel_Created
	IEventDataPtr e(GCC_NEW EvtData_Wheel_Created( this ) );
	m_em->VQueueEvent( e );
}

Wheel::~Wheel()
{}

void Wheel::StartMusicPlayer( IEventDataPtr pEventData )
{
	m_sound->playSound( "startMusicPlayer" );
	m_isMusicPanelOn = true;
}

void Wheel::RotateWheel()
{
	switch( m_wheelType )
	{
	case MusicWheel:
		{
			if( m_isMusicPanelOn )
			{
				switch( m_value )
				{
				case 1:
					m_sound->playSound( "blowSound2" );
					m_value = 2;
					break;
				case 2:
					m_sound->playSound( "blowSound3" );
					m_value = 3;
					break;
				case 3:
					m_sound->playSound( "blowSound4" );
					m_value = 4;
					break;
				case 4:
					m_sound->playSound( "blowSound1" );
					m_value = 1;
					break;
				}
			}

			break;
		}
	case BoilerWheel:
		{
			m_sound->playSound( "rotateSound" );
			if( !m_isOn )
				m_isOn = true;
			break;
		}
	case ButtonWheel:
		// Send evt
		break;
	}

	IEventDataPtr e(GCC_NEW EvtData_Rotate_Wheel( this ) );
	m_em->VQueueEvent( e );
}

int Wheel::GetValue() const
{
	if( m_wheelType == MusicWheel )
		return m_value;
	return 0;
}

bool Wheel::IsOn() const
{
	if( m_wheelType == BoilerWheel )
		return m_isOn;
	return 0;
}

void Wheel::ResetWheel()
{
	m_value = 1;
	m_isOn	= false;
}

WheelType Wheel::GetWheelType() const
{
	return m_wheelType;
}

std::string Wheel::GetName() const
{
	return m_name;
}


HRESULT Wheel::Update( float deltaTime, Camera* camera )
{
	HRESULT hr = S_OK;



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

MeshInfo Wheel::getInfo()
{
	return m_meshInfo;
}