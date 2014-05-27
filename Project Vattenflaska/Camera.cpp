/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
-------------------------------------------------------------------------
  History:
  2014-04-18 
  Created by Sebastian Genlund

*************************************************************************/

#include "Camera.h"

Camera::Camera()
	:	m_EyePos(  0.0f, 0.0f, 0.0f ),
		m_RightDirection( 1.0f, 0.0f, 0.0f ),
		m_UpDirection( 0.0f, 1.0f, 0.0f ),
		m_FocusPos( 0.0f, 0.0f, 1.0f ) 
{
	
	XMMATRIX V = XMMatrixLookAtLH( XMLoadFloat3( &m_EyePos ), XMLoadFloat3( &m_FocusPos ), XMLoadFloat3( &m_UpDirection ) );
	XMStoreFloat4x4( &m_View, V );

	XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(	0.95f,	  // field of view
											1280.0f / 720.0f,
											0.5f,	  // near
											5000.0f ); // far
	XMStoreFloat4x4( &m_Proj, P );
}

Camera::~Camera()
{}

void Camera::Strafe( float d )
{
	//// m_EyePos += d * m_RightDirection
	XMVECTOR s = XMVectorReplicate( d );
	XMVECTOR r = XMLoadFloat3( &m_RightDirection );
	XMVECTOR p = XMLoadFloat3( &m_EyePos );
	XMStoreFloat3( &m_EyePos, XMVectorMultiplyAdd( s, r, p ) );
}

void Camera::Walk( float d )
{
	// m_Eyepos += d * m_FocusPos
	XMVECTOR s = XMVectorReplicate( d );
	XMVECTOR l = XMLoadFloat3( &m_FocusPos );
	XMVECTOR p = XMLoadFloat3( &m_EyePos );
	XMStoreFloat3( &m_EyePos, XMVectorMultiplyAdd( s, l, p ) );

}
void Camera::setCameraPos(XMFLOAT3 pos)
{
	m_EyePos=pos;
}
XMFLOAT3 Camera::fakeWalk(float d)
{
	XMVECTOR s = XMVectorReplicate( d );
	XMVECTOR l = XMLoadFloat3( &m_FocusPos );
	XMVECTOR p = XMLoadFloat3( &m_EyePos );
	XMFLOAT3 returner;
	XMStoreFloat3(&returner, XMVectorMultiplyAdd(s, l, p));
	//XMStoreFloat3( &m_EyePos, XMVectorMultiplyAdd( s, l, p ) );
	return returner;
}
XMFLOAT3 Camera::fakeStrafe( float d )
{
	XMVECTOR s = XMVectorReplicate( d );
	XMVECTOR r = XMLoadFloat3( &m_RightDirection );
	XMVECTOR p = XMLoadFloat3( &m_EyePos );
	XMFLOAT3 returner;
	XMStoreFloat3( &returner, XMVectorMultiplyAdd( s, r, p ) );

	return returner;
}
void Camera::SetHeight( float y )
{
	m_EyePos.y = y+1.6f;
}

void Camera::Pitch( float angle )
{
	// Rotate up and look vector about the right vector

	XMMATRIX R = DirectX::XMMatrixRotationAxis( XMLoadFloat3( &m_RightDirection ), angle );

	XMStoreFloat3( &m_UpDirection, XMVector3TransformNormal( XMLoadFloat3( &m_UpDirection ), R ) );
	XMStoreFloat3( &m_FocusPos, XMVector3TransformNormal( XMLoadFloat3( &m_FocusPos ), R ) );
}

void Camera::RotateY( float angle )
{
	// Rotate the basis vectors about the world y-axis

	XMMATRIX R = DirectX::XMMatrixRotationY( angle );

	XMStoreFloat3( &m_RightDirection, XMVector3TransformNormal( XMLoadFloat3( &m_RightDirection ), R ) );
	XMStoreFloat3( &m_UpDirection, XMVector3TransformNormal(XMLoadFloat3( &m_UpDirection ), R ) );
	XMStoreFloat3( &m_FocusPos, XMVector3TransformNormal( XMLoadFloat3( &m_FocusPos ), R ) );
}

XMMATRIX Camera::View() const
{
	return XMLoadFloat4x4( &m_View );
}

XMMATRIX Camera::Proj() const
{
	return XMLoadFloat4x4( &m_Proj );
}

void Camera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3( &m_RightDirection );
	XMVECTOR U = XMLoadFloat3( &m_UpDirection );
	XMVECTOR L = XMLoadFloat3( &m_FocusPos );
	XMVECTOR P = XMLoadFloat3( &m_EyePos );

	// Keep camera's axes othogonal to each other and of unit length
	L = XMVector3Normalize( L );
	U = XMVector3Normalize( XMVector3Cross( L, R ) );

	// U, L already ortho-normal, so no need to normalize cross product
	R = XMVector3Cross( U, L );

	// Fill in the view matrix entries
	float x = -XMVectorGetX(XMVector3Dot( P, R ) );
	float y = -XMVectorGetX(XMVector3Dot( P, U ) );
	float z = -XMVectorGetX(XMVector3Dot( P, L ) );

	XMStoreFloat3( &m_RightDirection, R );
	XMStoreFloat3( &m_UpDirection, U );
	XMStoreFloat3( &m_FocusPos, L );

	m_View( 0, 0 ) = m_RightDirection.x;
	m_View( 1, 0 ) = m_RightDirection.y;
	m_View( 2, 0 ) = m_RightDirection.z;
	m_View( 3, 0) = x;

	m_View( 0, 1 ) = m_UpDirection.x;
	m_View( 1, 1 ) = m_UpDirection.y;
	m_View( 2, 1 ) = m_UpDirection.z;
	m_View( 3, 1 ) = y;

	m_View( 0, 2 ) = m_FocusPos.x;
	m_View( 1, 2 ) = m_FocusPos.y;
	m_View( 2, 2 ) = m_FocusPos.z;
	m_View( 3, 2 ) = z;

	m_View( 0, 3 ) = 0.0f;
	m_View( 1, 3 ) = 0.0f;
	m_View( 2, 3 ) = 0.0f;
	m_View( 3, 3 ) = 1.0f;
}

XMFLOAT4X4 Camera::GetProjMatrix() const
{
	XMMATRIX tempProj = XMLoadFloat4x4( &m_Proj );
	XMMatrixTranspose( tempProj );
	XMFLOAT4X4 returnMatrix;
	XMStoreFloat4x4( &returnMatrix, tempProj );

	return returnMatrix;
}

XMFLOAT4X4 Camera::GetViewMatrix() const
{
	XMMATRIX tempView = XMLoadFloat4x4( &m_View );
	XMMatrixTranspose( tempView );
	XMFLOAT4X4 returnMatrix;
	XMStoreFloat4x4( &returnMatrix, tempView );

	return returnMatrix;
}

XMVECTOR Camera::GetEyePos() const
{
	return XMLoadFloat3(&m_EyePos);
}

XMFLOAT3 Camera::GetEyePosAsFloat() const
{
	return m_EyePos;
}

float Camera::GetPosX() const
{
	return m_EyePos.x;
}

float Camera::GetPosZ() const
{
	return m_EyePos.z;
}

//
//XMVECTOR Camera::GetFocusPos() const
//{
//	return m_FocusPos;
//}
//
//void Camera::SetEyePos( XMVECTOR eyePos )
//{
//	m_EyePos = eyePos;
//}
//
//void Camera::SetFocusPos( XMVECTOR focusPos )
//{
//	m_FocusPos = focusPos;
//}