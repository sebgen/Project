/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
-------------------------------------------------------------------------
  History:
  2014-04-18 
  Created by Sebastian Genlund

*************************************************************************/

#include "Input.h"

//=============================================================================
// default constructor
//=============================================================================
Input::Input( )
{
	// Clear key down array
	for( int i = 0; i < inputNS::KEYS_ARRAY_LEN; i++ )
		keysDown[i] = false;

	// Clear key pressed array
	for( int i = 0; i < inputNS::KEYS_ARRAY_LEN; i++ )
		keysPressed[i] = false;

	// Mouse data
	m_MousePos.x		= 0;	
	m_MousePos.y		= 0;
	m_MousePosRaw.x		= 0;
	m_MousePosRaw.y		= 0;
	m_MouseLButton		= false;
	m_MouseRButton		= false;
}

//=============================================================================
// destructor
//=============================================================================
Input::~Input()
{
    if(m_MouseCaptured)
        ReleaseCapture();               // release mouse
}

//=============================================================================
// Initialize mouse
//=============================================================================
HRESULT Input::Initialize( HWND hwnd )
{
	// Register HD mouse
	m_Rid[0].usUsagePage	= HID_USAGE_PAGE_GENERIC;
	m_Rid[0].usUsage		= HID_USAGE_GENERIC_MOUSE;
	m_Rid[0].dwFlags		= RIDEV_INPUTSINK;
	m_Rid[0].hwndTarget		= hwnd;
	RegisterRawInputDevices( m_Rid, 1, sizeof(m_Rid[0]));

	return S_OK;
}


//==================================================================
//							MOUSE							 
//==================================================================

// FPS camera controls
// Not final
void Input::MouseMove( bool btnState, int x, int y, Camera* camera )
{
	//if( ( btnState & MK_RBUTTON ) != 0 )
	//{
	//	// Make each pixel correspond to a quarter of a degree
	//	float dx = XMConvertToRadians( 0.25f * static_cast<float>( x - m_MousePos.x ) );
	//	float dy = XMConvertToRadians( 0.25f * static_cast<float>( y - m_MousePos.y ) );

	//	camera->Pitch( dy );
	//	camera->RotateY( dx );
	//}

	//m_MousePos.x = x;
	//m_MousePos.y = y;

	if ( btnState)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = DirectX::XMConvertToRadians( 
							0.25f * static_cast<float>( x - m_tempPos.x ) );
		float dy = DirectX::XMConvertToRadians(
							0.25f * static_cast<float>( y - m_tempPos.y ) );

		camera->Pitch( dy );
		camera->RotateY( dx );
	}
	m_tempPos.x = x;
	m_tempPos.y = y;

	camera->UpdateViewMatrix();

}

// Reads mouse screen position into m_LastMousePos
void Input::MousePosition( LPARAM lParam )
{
	m_MousePos.x = GET_X_LPARAM( lParam );
	m_MousePos.y = GET_Y_LPARAM( lParam );
	//m_LastMousePos = lParam;
}

// Save state of mouse left button
void Input::SetMouseLButton( bool b )
{
	m_MouseLButton = b;
}

// Save state of mouse right button
void Input::SetMouseRButton( bool b )
{
	m_MouseRButton = b;
}

// Return m_LastMousePos
POINT Input::GetMousePos() const
{
	return m_MousePos;
}

// Return mouse X pos
int Input::GetMouseX() const
{
	return m_MousePos.x;
}

// Return mouse Y pos
int Input::GetMouseY() const
{
	return m_MousePos.y;
}

// Return state of of left mouse button
bool Input::GetMouseLButton() const
{
	return m_MouseLButton;
}

// Return state of right mouse button
bool Input::GetMouseRButton() const
{
	return m_MouseRButton;
}

// Reads raw mouse data into m_MousePosRaw
// This routine is compatible with a high-definition mouse
void Input::MousePositionRaw( LPARAM lParam )
{
	UINT dwSize = 40;
	static BYTE lpb[40];

	GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof( RAWINPUTHEADER ) );
	RAWINPUT* raw = (RAWINPUT*)lpb;

	if( raw->header.dwType == RIM_TYPEMOUSE )
	{
		m_MousePosRaw.x = raw->data.mouse.lLastX;
		m_MousePosRaw.y = raw->data.mouse.lLastY;
	}
}

// Return m_MousePosRaw
POINT Input::GetMousePosRaw() const
{
	return m_MousePosRaw;
}

// Return raw mouse X movement. Left is <0, Right is >0
// Compatible with high-definition mouse.
int  Input::GetMouseRawX() const
{
	return m_MousePosRaw.x;
}

// Return raw mouse Y movement. Up is <0, Down is >0
// Compatible with high-definition mouse.
int  Input::GetMouseRawY() const
{
	return m_MousePosRaw.y;
}

//==================================================================
//							Keyboard
//==================================================================

// Set true in the keysDown and keysPessed array for this key
// Pre: wParam contains the virtual key code (0--255)
void Input::KeyDown( WPARAM wParam )
{
	// make sure key code is within buffer range
	if( wParam < inputNS::KEYS_ARRAY_LEN )
	{
		keysDown[wParam]	= true;
		keysPressed[wParam] = true;
	}
}

// Set false in the keysDown array for this key
// Pre: wParam contains the virtual key code (0--255)
void Input::KeyUp( WPARAM wParam )
{
	// make sure key code is within buffer range
	if( wParam < inputNS::KEYS_ARRAY_LEN )
		keysDown[wParam] = false;
}

// Returns true if the specified VIRTUAL KEY is down, otherwise false.
bool Input::IsKeyDown( UCHAR vkey ) const
{
	if( vkey < inputNS::KEYS_ARRAY_LEN )
		return keysDown[vkey];
	else
		return false;
}

// Return true if the specified VIRTUAL KEY has been pressed in the most recent frame.
// Key presses are erased at the end of each frame.
bool Input::WasKeyPressed( UCHAR vkey ) const
{
	if( vkey < inputNS::KEYS_ARRAY_LEN )
		return keysPressed[vkey];
	else
		return false;
}

// Return true if any key was pressed in the most recent frame.
// Key presses are erased at the end of each frame.
bool Input::AnyKeyPressed() const
{
	for( int i = 0; i < inputNS::KEYS_ARRAY_LEN; i++ )
	{
		if( keysPressed[i] == true )
		return true;
	}
	return false;
}

// Clear the specified key press
void Input::ClearKeyPress( UCHAR vkey )
{
	if( vkey < inputNS::KEYS_ARRAY_LEN )
		keysPressed[vkey] = false;
}

// Clear specified input buffers
// See input.h for what values
void Input::Clear( UCHAR what )
{
	if( what & inputNS::KEYS_DOWN )		// if clear keys down
	{
		for( int i = 0; i < inputNS::KEYS_ARRAY_LEN; i++ )
			keysDown[i] = false;
	}

	if( what & inputNS::KEYS_PRESSED )	// if clear keys pressed
	{
		for( int i = 0; i < inputNS::KEYS_ARRAY_LEN; i++ )
			keysPressed[i] = false;
	}

	if( what & inputNS::MOUSE )			// if clear mouse
	{
		m_MousePos.x = 0;
		m_MousePos.y = 0;
		m_MousePosRaw.x = 0;
		m_MousePosRaw.y = 0;
	}
}