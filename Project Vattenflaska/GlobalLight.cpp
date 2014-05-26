#include "GlobalLight.h"
#include <stdlib.h>

GlobalLight::GlobalLight()
{

}

GlobalLight::GlobalLight( XMFLOAT4& diffuse, XMFLOAT3& position, float& range )
{
	m_intensity = (float)rand()/(float)(RAND_MAX);
	
	if( m_intensity < 0.2f )
		m_intensity += 0.2f;

	m_diffuse = diffuse; 
	m_positionAndRange = XMFLOAT4( position.x, position.y, position.z, range);
}

GlobalLight::~GlobalLight()
{}

void GlobalLight::Update( float deltaTime )
{
	if( m_positionAndRange.w <= m_intensity)
	{
		m_positionAndRange.w += deltaTime * 1.5f;
		if( m_positionAndRange.w >= m_intensity )
		{
			m_intensity = (float)rand()/(float)(RAND_MAX);
			if( m_intensity < 0.4f )
				m_intensity += 0.4f;
		}
	}

	if( m_positionAndRange.w >= m_intensity )
	{
		m_positionAndRange.w -= deltaTime * 1.5f;
		if( m_positionAndRange.w <= m_intensity )
		{
			m_intensity = (float)rand()/(float)(RAND_MAX);
			if( m_intensity < 0.4f )
				m_intensity += 0.4f;
		}
	}
}