/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Application.h
  The main application: initialize, process input, update and render game
-------------------------------------------------------------------------
  History:
  2014-04-?
  Created by Simon Johansson
  -----------------------------------------------------------------------
  2014-04-20
  Edited by Sebastian Genlund
  *Löst att WndProc är static genom att kalla den från en global Application
	instance från MainWndProc. ( nu kan vi använda ostatiska variabler i den).
  *Skapat HandleInput( UINT msg, WPARAM wParam, LPARAM lParam ) som kallas 
   från Run() för att hantera och uppdatera input. Kanske lägger denna i
   själva Input klassen istället
*************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#pragma comment( lib, "d3d11.lib" )


#include "3DLibs.h"
#include "Input.h"
#include "GameTime.h"

const unsigned int SCREEN_WIDTH		=	1280;
const unsigned int SCREEN_HEIGHT	=	720;

class Application
{
	protected:
		//--------
		// Window
		//--------
		HINSTANCE	m_hInstance;
		HWND		m_hWnd;

		//------------
		// DirectX 11
		//------------
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		IDXGISwapChain*			m_swapChain;
		ID3D11RenderTargetView*	m_renderTargetView;
		ID3D11Texture2D*		m_depthStencil;
		ID3D11DepthStencilView*	m_depthStencilView;

		//--------
		// Game
		//--------
		Input*		m_input;
		GameTime*	m_gameTime;

	protected:
		HRESULT Update( float deltaTime );
		HRESULT Draw( float deltaTime );

		void HandleInput( UINT msg, WPARAM wParam, LPARAM lParam);
		void HandleMovementBools(bool& isWalking, bool& isForward, bool& isBackward, bool& isStrafing, bool& isRight, bool& isLeft, bool& isRotating, bool& isRotatingLeft, bool& isRotatingRight );
	public:
		LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		Application();
		~Application();

		HRESULT InitializeWindow( HINSTANCE hInstance, int nCmdShot );
		HRESULT InitializeDirectX11();
		HRESULT InitializeInput();

		void Shutdown();
};
#endif
