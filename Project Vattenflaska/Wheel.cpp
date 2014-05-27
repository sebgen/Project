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

	// Send event: EvtData_Wheel_Created
	IEventDataPtr e(GCC_NEW EvtData_Wheel_Created( this ) );
	m_em->VQueueEvent( e );
}

Wheel::~Wheel()
{}

void Wheel::RotateWheel()
{
	switch( m_wheelType )
	{
	case MusicWheel:
		{
			switch( m_value )
			{
			case 1:
			case 2:
			case 3:
				m_value++;
				break;
			case 4:
				m_value = 1;
				break;
			}
			break;
		}
	case BoilerWheel:
		{
			if( m_isOn )
				m_isOn = false;
			else
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