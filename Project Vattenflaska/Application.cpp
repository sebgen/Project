/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
-------------------------------------------------------------------------
  History:
  2014-04-? 
  Created by Simon Johansson

*************************************************************************/

#include "Application.h"

HRESULT Application::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT Application::Draw( float deltaTime )
{
	HRESULT hr = S_OK;

	return hr;
}

namespace
{
	Application* g_app = 0;
}

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_app->WndProc( hwnd, msg, wParam, lParam );
}

LRESULT CALLBACK Application::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( msg )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			return 0;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;

		case WM_KEYDOWN:
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
			}
			return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void Application::HandleInput( UINT msg, WPARAM wParam, LPARAM lParam, float deltaTime, bool& isWalking, bool& isForward, bool& isBackward, bool& isStrafing, bool& isRight, bool& isLeft, bool& isRotating, bool& isOpening, bool& isClosing )
{
	//switch ( msg )
	//	{

	//			// Key down
	//		case WM_KEYDOWN: case WM_SYSKEYDOWN:
	//			m_input->KeyDown( wParam );
	//			if( GetAsyncKeyState( 'W' ) )
	//				m_scene->GetCamera()->Walk( 10 );
	//			return;

	//			// Key up
	//		case WM_KEYUP: case WM_SYSKEYUP:
	//			m_input->KeyUp( wParam );
	//			return;

	//			// Mouse move
	//		case WM_MOUSEMOVE:
	//			m_input->MousePosition( lParam );
	//			return;

	//			// Mouse move raw
	//		case WM_INPUT:
	//			//m_input->MousePositionRaw( lParam );
	//			return;

	//			// Mouse left button down
	//		case WM_LBUTTONDOWN:                   
 //               m_input->SetMouseLButton( true );
	//			m_input->MousePosition( lParam );             
 //               return;

	//			// Mouse left button up
 //           case WM_LBUTTONUP:                     
 //               m_input->SetMouseLButton( false );
	//			m_input->MousePosition( lParam );             
 //               return;

	//			// Mouse right button down
	//		case WM_RBUTTONDOWN:                    
 //               m_input->SetMouseRButton( true );
 //               m_input->MousePosition( lParam );             
 //               return;

	//			// Mouse right button up
 //           case WM_RBUTTONUP:                      
 //               m_input->SetMouseRButton( false );
 //               m_input->MousePosition( lParam );             
 //               return;
	//}


	//// Input test 
	//if( GetAsyncKeyState('W') & 0x8000 )
	//	m_scene->GetCamera()->Walk( 5.0f * deltaTime ); 

	//if( GetAsyncKeyState('S') & 0x8000 )
	//	m_scene->GetCamera()->Walk( -5.0f * deltaTime );

	//if( GetAsyncKeyState('A') & 0x8000 ) 
	//	m_scene->GetCamera()->Strafe( -5.0f * deltaTime );
	//
	//if( GetAsyncKeyState('D') & 0x8000 )
	//	m_scene->GetCamera()->Strafe( 5.0f * deltaTime );

		// Input test 
	if( GetAsyncKeyState('W') & 0x8000 )
	{
		isWalking = true;
		isForward = true;
	}

	if( GetAsyncKeyState('S') & 0x8000 )
	{
		isWalking  = true;
		isBackward = true;
	}

	if( GetAsyncKeyState('A') & 0x8000 ) 
	{
		isStrafing = true;
		isLeft	   = true;
	}
	
	if( GetAsyncKeyState('D') & 0x8000 )
	{
		isStrafing = true;
		isRight	   = true;
	}

	if( GetAsyncKeyState('E') & 0x8000 )
	{
		isRotating = true;
		isRight	   = true;
	}

	if( GetAsyncKeyState('Q') & 0x8000 )
	{
		isRotating = true;
		isLeft	   = true;
	}

	if( GetAsyncKeyState('G') & 0x8000 )
	{
		isOpening = true;
	}

	if( GetAsyncKeyState('C') & 0x8000 )
	{
		isClosing = true;
	}
}

Application::Application()
{
	//--------
	// Window
	//--------
	m_hInstance		= NULL;
	m_hWnd			= NULL;


	//--------
	// Game
	//--------
	m_input			= NULL;
	m_gameTime		= NULL;
}

Application::~Application()
{

}

HRESULT Application::InitializeWindow( HINSTANCE hInstance, int nCmdShow )
{
	HRESULT hr = S_OK;

	m_hInstance = hInstance;

	//----------------------
	// Register Window Class
	//----------------------
	WNDCLASSEX wc;
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
	wc.hIcon			= 0;
	wc.hCursor			= LoadCursor( NULL, IDC_HAND );
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "Game";
	wc.hIconSm			= 0;

	if( FAILED( hr = RegisterClassEx( &wc ) ) )
		return hr;

	//-----------------------
	// Adjust & Create Window
	//-----------------------
	RECT rc = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if( !( m_hWnd = CreateWindow(	"Game",
									"Game",
									WS_OVERLAPPEDWINDOW,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									rc.right - rc.left,
									rc.bottom - rc.top,
									NULL,
									NULL,
									m_hInstance,
									NULL ) ) )
	{
		return E_FAIL;
	}

	ShowWindow( m_hWnd, nCmdShow );
	ShowCursor( TRUE );

	return hr;
}

HRESULT Application::InitializeDirectX11()
{
	HRESULT hr = E_FAIL;

	RECT rc;
	GetClientRect( m_hWnd, &rc );

	int width	= rc.right - rc.left;
	int height	= rc.bottom - rc.top;


	//-------------------------------------------
	// Create Swap Chain, Device & Device Context
	//-------------------------------------------

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,
									  D3D_DRIVER_TYPE_WARP,
									  D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC sd;
	memset( &sd, 0, sizeof( sd ) );
	sd.BufferCount				= 1;
	sd.BufferDesc.Width						= width;
	sd.BufferDesc.Height					= height;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= m_hWnd;
	sd.SampleDesc.Count						= 1;
	sd.Windowed								= TRUE;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for ( UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr ); driverTypeIndex++ )
	{
		hr = D3D11CreateDeviceAndSwapChain( nullptr,
											driverTypes[driverTypeIndex],
											NULL,
											0,
											featureLevelsToTry,
											ARRAYSIZE( featureLevelsToTry ),
											D3D11_SDK_VERSION,
											&sd,
											&m_swapChain,
											&m_device,
											&initiatedFeatureLevel,
											&m_deviceContext );
	}

	if( FAILED( hr ) )
		return hr;


	//--------------------------
	// Create Render Target View
	//--------------------------

	ID3D11Texture2D* pBackBuffer;
	if ( SUCCEEDED( hr = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer ) ) )
	{
		hr = m_device->CreateRenderTargetView( pBackBuffer, nullptr, &m_renderTargetView );
		pBackBuffer->Release();
	}
	
	if( FAILED( hr ) )
		return hr;


	//--------------------------
	// Create Depth Stencil View
	//--------------------------

	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width				= width;
	dsd.Height				= height;
	dsd.MipLevels			= 1;
	dsd.ArraySize			= 1;
	dsd.Format				= DXGI_FORMAT_D32_FLOAT;
	dsd.SampleDesc.Count	= 1;
	dsd.SampleDesc.Quality	= 0;
	dsd.Usage				= D3D11_USAGE_DEFAULT;
	dsd.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags		= 0;
	dsd.MiscFlags			= 0;

	if( FAILED( hr = m_device->CreateTexture2D( &dsd, nullptr, &m_depthStencil ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= dsd.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice	= 0;

	if( FAILED( hr = m_device->CreateDepthStencilView( m_depthStencil, &dsvd, &m_depthStencilView ) ) )
		return hr;

	//-------------
	// Set Viewport
	//-------------

	D3D11_VIEWPORT vp;
	vp.Width	= (float)width;
	vp.Height	= (float)height;
	vp.MinDepth	= 0.0f;
	vp.MaxDepth	= 1.0f;
	vp.TopLeftX	= 0;
	vp.TopLeftY	= 0;
	m_deviceContext->RSSetViewports( 1, &vp );

	m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView );

	return hr;
}

HRESULT Application::InitializeInput()
{
	HRESULT hr = S_OK;

	m_input = new Input();

	if( FAILED( hr = m_input->Initialize( m_hWnd ) ) )
		return hr;

	return hr;
}

//int Application::Run()
//{
//	MSG msg = {0};
//	POINT p;
//	// Reset the timer
//	m_gameTime->Reset();
//
//
//	//-----CAMERA MOVEMENT---------
//	bool isWalking   = false;
//	bool isForward	 = false;
//	bool isBackward  = false;
//
//	bool isStrafing  = false;
//	bool isRight	 = false;
//	bool isLeft		 = false;
//
//	bool isRotating  = false;
//
//	float moveFactor = 0.5f;
//	float moveUnits  = 10.0f;
//	//-----------------------------
//
//	//-------GATE CONTROLS---------
//	bool  isOpening = false;
//	bool  isClosing = false;
//	float gateUnits = 220.0f;
//	//-----------------------------
//
//	while ( WM_QUIT != msg.message )
//	{
//		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
//		{
//			TranslateMessage( &msg );
//			DispatchMessage ( &msg );
//		}
//		else
//		{
//				
//			GetCursorPos( &p );
//			m_input->MouseMove( msg.wParam, p.x, p.y, m_scene->GetCamera() );
//
//			// See GameTime.cpp for implementation
//			m_gameTime->Tick();
//
//			/// TEST
//			//================================================================================
//			if ( isWalking )
//			{
//				if( isForward )
//				{
//					m_scene->GetCamera()->Walk( moveFactor );
//					if ( moveUnits <= 0 )
//					{
//						moveUnits = 10.0f;
//						isWalking = false;
//						isForward = false;
//					}
//					else
//						moveUnits--;
//				}
//				else if( isBackward )
//				{
//					m_scene->GetCamera()->Walk( -moveFactor );
//					if ( moveUnits <= 0 )
//					{
//						moveUnits  = 10.0f;
//						isWalking  = false;
//						isBackward = false;
//					}
//					else
//						moveUnits--;
//				}
//			}
//
//
//			else if( isStrafing )
//			{
//				if( isRight )
//				{
//					m_scene->GetCamera()->Strafe( moveFactor );
//					if ( moveUnits <= 0 )
//					{
//						moveUnits = 10.0f;
//						isStrafing = false;
//						isRight    = false;
//					}
//					else
//						moveUnits--;
//				}
//				else if( isLeft )
//				{
//					m_scene->GetCamera()->Strafe( -moveFactor );
//					if ( moveUnits <= 0 )
//					{
//						moveUnits  = 10.0f;
//						isStrafing = false;
//						isLeft	   = false;
//					}
//					else
//						moveUnits--;
//				}
//			}
//
//			else if( isRotating )
//			{
//				double radians = (XM_PIDIV2 / 10);
//				if( isRight )
//				{
//					;
//					m_scene->GetCamera()->RotateY( radians );
//					if ( moveUnits <= 1 )
//					{
//						moveUnits  = 10.0f;
//						isRotating = false;
//						isRight    = false;
//					}
//					else
//						moveUnits--;
//				}
//				else if( isLeft )
//				{
//					m_scene->GetCamera()->RotateY( -radians );
//					if ( moveUnits <= 1 )
//					{
//						moveUnits  = 10.0f;
//						isRotating = false;
//						isRight    = false;
//					}
//					else
//						moveUnits--;
//				}
//			}
//			else
//			{
//				// Process input
//				HandleInput( msg.message, msg.wParam, msg.lParam, m_gameTime->GetDeltaTime(), isWalking, isForward, isBackward, isStrafing, isRight, isLeft, isRotating, isOpening, isClosing );
//			}
//
//			//---------- GATE ---------------
//			if( isOpening )
//			{
//				m_scene->GetGate()->ChangeYCoordinate( 0.01f );
//				if( gateUnits <= 0 )
//				{
//					gateUnits = 220.0f;
//					isOpening = false;
//				}
//				else
//					gateUnits--;			
//			}
//
//			if( isClosing )
//			{
//				m_scene->GetGate()->ChangeYCoordinate( -0.10f );
//				if( gateUnits <= 0 )
//				{
//					gateUnits = 220.0f;
//					isClosing = false;
//				}
//				else
//					gateUnits -= 10.0f;			
//			}
//			//----------------------------------------
//
//			//================================================================================
//
//			// Lock height every frame
//			m_scene->GetCamera()->SetHeight( 1.0 );
//
//			// Update game logic
//			Update( m_gameTime->GetDeltaTime() );
//
//			// Render frame
//			Render();
//		}
//	}
//
//	return (int)msg.wParam;
//}

void Application::Shutdown()
{
	SAFE_RELEASE( m_device );
	SAFE_RELEASE( m_deviceContext );
	SAFE_RELEASE( m_swapChain );
	SAFE_RELEASE( m_renderTargetView );
	SAFE_RELEASE( m_depthStencil );
	SAFE_RELEASE( m_depthStencilView );

	SAFE_DELETE( m_input );
	SAFE_DELETE( m_gameTime );
}