/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Input.h
  Read input from keyboard and mouse

-------------------------------------------------------------------------
  History:
  2014-04-18 
  Created by Sebastian Genlund

*************************************************************************/

#ifndef _INPUT_H_
#define _INPUT_H_

#include "3DLibs.h"
#include "Camera.h"

// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif
#ifndef RAW_INPUT_KEYBOARD
#define RAW_INPUT_KEYBOARD			((USHORT) 0x06)
#endif
//--------------------------

namespace inputNS
{
    const int KEYS_ARRAY_LEN = 256;     // size of key arrays
    
    // what values for clear(), bit flag
    const UCHAR KEYS_DOWN = 1;
    const UCHAR KEYS_PRESSED = 2;
    const UCHAR MOUSE = 4;
}

class Input
{
private:
	bool keysDown[inputNS::KEYS_ARRAY_LEN];     // true if specified key is down
    bool keysPressed[inputNS::KEYS_ARRAY_LEN];  // true if specified key was pressed

	POINT m_MousePos;			// Mouse screen coordinates
	POINT m_MousePosRaw;		// high-definition mouse data
	POINT m_tempPos;
	RAWINPUTDEVICE m_Rid[1];		// for high-definition mouse (change to 2 for hd keyboard and use "m_Rid[1].usUsage = RAW_INPUT_KEYBOARD")
	bool m_MouseCaptured;		// true if mouse captured
	bool m_MouseLButton;		// True if left mouse button down
	bool m_MouseRButton;		// True if right mouse button down

public:
	Input();
	virtual ~Input();

	HRESULT Initialize( HWND hwnd );

//==================================================================
//							MOUSE							 
//==================================================================

	// FPS camera controls
	void MouseMove( bool btnState, int x, int y, Camera* camera );

	// Reads mouse screen position into m_MousePos
	void MousePosition( LPARAM lParam );

	// Save state of mouse left button
	void SetMouseLButton( bool b );

	// Save state of mouse right button
	void SetMouseRButton( bool b );

	// Return m_MousePos
	POINT GetMousePos() const;

	// Return mouse X pos
	int GetMouseX() const;

	// Return mouse Y pos
	int GetMouseY() const;

	// Return state of of left mouse button
	bool GetMouseLButton() const;

	// Return state of right mouse button
	bool GetMouseRButton() const;

	//========================
	//		MOUSE - RAW							 
	//========================

	// Reads raw mouse data into m_MousePosRaw
    // This routine is compatible with a high-definition mouse
    void MousePositionRaw( LPARAM lParam );

	// Return m_MousePosRaw
    POINT GetMousePosRaw() const;

	// Return raw mouse X movement. Left is <0, Right is >0
    // Compatible with high-definition mouse.
    int  GetMouseRawX() const;

	// Return raw mouse Y movement. Up is <0, Down is >0
    // Compatible with high-definition mouse.
    int  GetMouseRawY() const;



//==================================================================
//							Keyboard
//==================================================================

	// Save key down state
	void KeyDown( WPARAM wParam );

	// Save key up state
	void KeyUp( WPARAM wParam );

	// Returns true if the specified VIRTUAL KEY is down, otherwise false.
    bool IsKeyDown( UCHAR vkey ) const;

	// Return true if the specified VIRTUAL KEY has been pressed in the most recent frame.
    // Key presses are erased at the end of each frame.
    bool WasKeyPressed( UCHAR vkey ) const;

	// Return true if any key was pressed in the most recent frame.
    // Key presses are erased at the end of each frame.
    bool AnyKeyPressed() const;

	// Clear the specified key press
    void ClearKeyPress( UCHAR vkey );

	// Clear specified input buffers where what is any combination of
    // KEYS_DOWN, KEYS_PRESSED or MOUSE.
    // Use OR '|' operator to combine parmeters.
    void Clear( UCHAR what );
};

#endif