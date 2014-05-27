/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Camera.h
  FPS - camera
  From Frank Luna's Introduction to 3D Game Programming with DirectX 11
-------------------------------------------------------------------------
  History:
  2014-04-18 
  Created by Sebastian Genlund

*************************************************************************/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "3DLibs.h"
using namespace DirectX;

class Camera
{
private:
	//XMMATRIX m_View;	// Flyttar världen för kameran
	//XMMATRIX m_Proj;	// Projeserar världen till skärmen

	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

	XMFLOAT3 m_EyePos;
	XMFLOAT3 m_FocusPos;
	XMFLOAT3 m_UpDirection;
	XMFLOAT3 m_RightDirection;


public:
	Camera();
	virtual ~Camera();

	// Move
	void Strafe( float d );
	void Walk( float d );

	void setCameraPos(XMFLOAT3 pos);
	//fake move
	XMFLOAT3 fakeWalk(float d);
	XMFLOAT3 fakeStrafe(float d);

	void SetHeight( float y );

	// Rotate
	void Pitch( float angle );
	void RotateY( float angle );

	XMMATRIX View() const;
	XMMATRIX Proj() const;

	void UpdateViewMatrix();

	XMFLOAT4X4 GetViewMatrix() const;
	XMFLOAT4X4 GetProjMatrix() const;
	
	XMVECTOR GetEyePos() const;
	XMFLOAT3 GetEyePosAsFloat() const;
	float GetPosX() const;
	float GetPosZ() const;
	//XMVECTORwcusPos() const;
	
	//void SetEyePos( XMVECTOR eyePos );
	//void SetFocusPos( XMVECTOR focusPos );

};

#endif