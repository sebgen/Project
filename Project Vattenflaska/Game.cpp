#include "Game.h"

HRESULT Game::Update( float deltaTime )
{
	HRESULT hr = S_OK;
	POINT pos= m_input->GetMousePos();

	UpdateCbCamera();
	UpdateCbLight( deltaTime );  /// NY

	

	m_currentRoom->Update( deltaTime, m_camera );

	handleMovement(deltaTime);
	m_input->MouseMove(m_input->GetMouseLButton(),pos.x, pos.y,m_camera);

	return hr;
}
void Game::handleMovement(float deltaTime)
{
	
	/// TEST
			//================================================================================
			if ( isWalking )
			{
				animationTimer +=deltaTime;

				if( isForward )
				{
					if( animationTimer >= 0.01f )
					{
						//m_navMesh->moveForward(moveFactor);
						m_camera->Walk( moveFactor );
						animationTimer = 0.0f;

						if ( moveUnits <= 1  )
						{
							moveUnits = nrOfMovement;
							//m_navMesh->moveDone();
							isWalking = false;
							isForward = false;
						}
						else
						{
							moveUnits--;
						
						}
							
					}			
				}
				else if( isBackward )
				{
					if( animationTimer >= 0.01f )
					{
						m_camera->Walk( -moveFactor );
						//m_navMesh->moveBackWard(moveFactor);
						animationTimer = 0.0f;

						if ( moveUnits <= 1 )
						{
							moveUnits  = nrOfMovement;
							//m_navMesh->moveDone();
							isWalking  = false;
							isBackward = false;
						}
						else
						{
							moveUnits--;
							
						}
					}
				}
			}


			else if( isStrafing )
			{
				animationTimer += m_gameTime->GetDeltaTime();

				if( isRight )
				{
					if( animationTimer >= 0.01f )
					{

						//m_navMesh->moveRight(moveFactor);
						m_camera->Strafe( moveFactor );
						animationTimer = 0.0f;

						if ( moveUnits <= 1 )
						{
							moveUnits = nrOfMovement;
							//m_navMesh->moveDone();
							isStrafing = false;
							isRight    = false;
						}
						else
						{
							moveUnits--;
							
						}
					}
				}
				else if( isLeft )
				{
					if( animationTimer >= 0.01f )
					{

						//m_navMesh->moveLeft(moveFactor);
						m_camera->Strafe( -moveFactor );
						animationTimer = 0.0f;

						if ( moveUnits <= 1 )
						{
							moveUnits  = nrOfMovement;
							//m_navMesh->moveDone();
							isStrafing = false;
							isLeft	   = false;
						}
						else
						{
							moveUnits--;
							
						}
					}
				}
			}

			else if( isRotating )
			{
				animationTimer +=deltaTime;

				if( isRotatingLeft )
				{
					if( animationTimer >= 0.01f )
					{
						m_camera->RotateY( XMConvertToRadians(-rotationDegree));
						animationTimer=0;

						if ( moveUnits <= 1 )
						{
							moveUnits  = nrOfMovement;
							isRotating = false;
							isRotatingLeft = false;
							//m_testLever->IsRotating( false );
						}
						else
							moveUnits--;
					}
				}
				else if( isRotatingRight )
				{
					if( animationTimer >= 0.01f )
					{
						m_camera->RotateY( XMConvertToRadians(rotationDegree));
						animationTimer=0;
						if ( moveUnits <= 1 )
						{
							
							moveUnits  = nrOfMovement;
							isRotating = false;
							isRotatingRight    = false;
						}
						else
							moveUnits--;
					}
				}
			}
			else
			{
				// Process input
				//OutputDebugString("inne i handlemovement\n");
				HandleMovementBools(isWalking, isForward, isBackward, isStrafing, isRight, isLeft, isRotating, isRotatingLeft, isRotatingRight );
			}
}
HRESULT Game::Draw( float deltaTime )
{
	HRESULT hr = S_OK;

	
	// Clear Back Buffer
	static float clearColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_renderTargetView, clearColor );

	// Clear Depth Buffer
	m_deviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	//========== START ===============

	m_deviceContext->RSSetState( m_rasterizerState );

	//---------------
	// Light Buffer |
	//---------------
	m_deviceContext->VSSetConstantBuffers( 0, 1, &m_cbCamera );
	m_deviceContext->PSSetConstantBuffers( 0, 1, &m_cbLight );  /// NY
	
	// Set topology
	m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->IASetInputLayout( m_inputLayout );

	// Set shader stages
	m_deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
	m_deviceContext->HSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->DSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->GSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );
	//---------------------------------------------

	// Render Room
	m_currentRoom->Draw( deltaTime );

	//========== END ===============

	// Swap Front and Back Bufffer
	return m_swapChain->Present( 0, 0 );

	return hr;
}

HRESULT Game::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags =	D3DCOMPILE_ENABLE_STRICTNESS | 
							D3DCOMPILE_IEEE_STRICTNESS;

	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );

	if ( in )
	{
		in.seekg( 0, std::ios::end );
		shader_code.resize( (unsigned int)in.tellg() );
		in.seekg( 0, std::ios::beg );
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompile( shader_code.data(),
							 shader_code.size(),
							 NULL,
							 pDefines,
							 nullptr,
							 pEntrypoint,
							 pTarget,
							 dwShaderFlags,
							 NULL,
							 pCompiledShader,
							 &pErrorBlob );

	if( pErrorBlob )
	{
		OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
		SAFE_RELEASE( pErrorBlob );
	}


	return hr;
}

HRESULT Game::InitializeShaders()
{
	HRESULT hr = S_OK;

	//----------------------
	// Compile Vertex Shader
	//----------------------

	ID3DBlob* vs = nullptr;
	
	if ( SUCCEEDED( hr = CompileShader( "VertexShader.sh", "VS_main", "vs_4_0", nullptr, &vs ) ) )
	{
		if( SUCCEEDED( hr = m_device->CreateVertexShader( vs->GetBufferPointer(),
														  vs->GetBufferSize(),
														  nullptr,
														  &m_vertexShader ) ) )
		{
			D3D11_INPUT_ELEMENT_DESC inputDesc[] = {				 
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hr = m_device->CreateInputLayout( inputDesc,
									     	  ARRAYSIZE( inputDesc ),
											  vs->GetBufferPointer(),
											  vs->GetBufferSize(),
											  &m_inputLayout );
		}

		SAFE_RELEASE( vs );
	}


	//---------------------
	// Compile Pixel Shader
	//---------------------

	ID3DBlob* ps = nullptr;

	if( SUCCEEDED( hr = CompileShader( "PixelShader.sh", "PS_main", "ps_4_0", nullptr, &ps ) ) )
	{
		hr = m_device->CreatePixelShader( ps->GetBufferPointer(),
										  ps->GetBufferSize(),
										  nullptr,
										  &m_pixelShader );
		SAFE_RELEASE( ps );
	}

	return hr;
}

HRESULT Game::CreateCbLightBuffer()  /// NY
{
	for (int i = 0; i < m_currentRoom->GetLights().size(); i++)
		m_lights.push_back( m_currentRoom->GetLights().at(i) );

	m_LightData.nrOfLights = m_lights.size();
	memcpy( m_LightData.lights, &m_lights[0], sizeof(GlobalLight) * m_lights.size() );
	

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof( m_LightData );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = m_device->CreateBuffer( &cbDesc, nullptr, &m_cbLight );

	return hr;
}

void Game::UpdateCbLight( float deltaTime )  /// NY
{
	for (int i = 0; i < m_lights.size(); i++)
		m_LightData.lights[i].Update( deltaTime );
	
	XMStoreFloat4( &m_LightData.cameraPosition, m_camera->GetEyePos() ); 

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_deviceContext->Map( m_cbLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if(SUCCEEDED(hr))
	{
		memcpy(mappedResource.pData, &m_LightData, sizeof(m_LightData));
	
		m_deviceContext->Unmap( m_cbLight, 0 );
	}


}

HRESULT Game::CreateCbCameraBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof( m_CameraData );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	HRESULT hr = m_device->CreateBuffer( &cbDesc, nullptr, &m_cbCamera );

	return hr;
}

void Game::UpdateCbCamera()
{
	XMStoreFloat4x4( &m_CameraData.viewMatrix, XMLoadFloat4x4( &m_camera->GetViewMatrix() ));
	XMStoreFloat4x4( &m_CameraData.projectionMatrix, XMLoadFloat4x4( &m_camera->GetProjMatrix() ));
	CXMMATRIX VP = XMMatrixMultiply( XMLoadFloat4x4( &m_camera->GetViewMatrix() ) , XMLoadFloat4x4( &m_camera->GetProjMatrix()) );
	XMStoreFloat4x4( &m_CameraData.viewProjectionMatrix, XMMatrixTranspose(VP) );


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_deviceContext->Map( m_cbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if(SUCCEEDED(hr))
	{
		memcpy(mappedResource.pData, &m_CameraData, sizeof(m_CameraData));
	
		m_deviceContext->Unmap( m_cbCamera, 0 );
	}
}

Game::Game()
	:Application()
{
	m_rasterizerState	= nullptr;
	m_inputLayout		= nullptr;
	m_matrixBuffer		= nullptr;
	m_vertexShader		= nullptr;
	m_pixelShader		= nullptr;

	m_importReader		= nullptr;
	m_currentRoom		= nullptr;
	m_camera			= nullptr;
}

Game::~Game()
{}

HRESULT Game::InitializeGame()
{
	HRESULT hr = S_OK;

	//--------------------------
	// Create Rasterizer State |
	//--------------------------
	D3D11_RASTERIZER_DESC rd;
	memset( &rd, 0, sizeof( rd ) );
	rd.FillMode			= D3D11_FILL_SOLID;
	rd.CullMode			= D3D11_CULL_NONE;

	hr = m_device->CreateRasterizerState( &rd, &m_rasterizerState );

	m_inputLayout = nullptr;


	//----------------------------
	// Initialize Game Resources |
	//----------------------------
	m_gameTime = new GameTime();
	m_camera = new Camera();
	InitializeShaders();



	//=================================
	//          LOAD LEVELS          ||
	//=================================
	m_importReader = new ImportReader();

	//---------------------
	// Load Torture Level |
	//---------------------
	m_importReader->LoadObject( m_device, m_deviceContext, m_rooms, "tortureLevelFirstDraft" );
	m_currentRoom = m_rooms.at(0);

	CreateCbLightBuffer();  /// NY
	CreateCbCameraBuffer();

	return hr;
}

int Game::Run()
{
	MSG msg = {0};
	POINT p;

	// Reset the timer
	m_gameTime->Reset();


	//-----CAMERA MOVEMENT---------
	 isWalking   = false;
	 isForward	 = false;
	 isBackward  = false;

	 isStrafing  = false;
	 isRight	 = false;
	 isLeft		 = false;


	 isRotating  = false;
	 isRotatingLeft= false;
	 isRotatingRight= false;

	 moveUnits  = 20.0f;
	 moveFactor = 1/moveUnits;
	 nrOfMovement = moveUnits;
	 rotationDegree=90/moveUnits;
	//-----------------------------

	//-------GATE CONTROLS---------
	bool  isOpening = false;
	bool  isClosing = false;
	float gateUnits = 220.0f;
	//-----------------------------

	//---------ANIMATION-----------
	 animationTimer = 0.0f;
	//-----------------------------


	while ( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}
		else
		{
				
			//GetCursorPos( &p );
			//m_input->MouseMove( msg.wParam, p.x, p.y, m_camera );


			// See GameTime.cpp for implementation
			m_gameTime->Tick();

		
			////---------- GATE ---------------
			//if( isOpening )
			//{
			//	if( !m_currentRoom->GetDoor()->IsOpen() )
			//	{
			//		m_currentRoom->GetDoor()->OpenDoor();
			//		isOpening = false;
			//	}			
			//}

			//if( isClosing )
			//{
			//	//m_scene->GetGate()->ChangeYCoordinate( -0.10f );
			//	if( gateUnits <= 0 )
			//	{
			//		gateUnits = 220.0f;
			//		isClosing = false;
			//	}
			//	else
			//		gateUnits -= 10.0f;			
			//}
			//----------------------------------------

			//================================================================================

			// Lock height every frame
			m_camera->SetHeight( 1.6f );

			HandleInput( msg.message, msg.wParam, msg.lParam );
			// Update game logic
			Update( m_gameTime->GetDeltaTime() );

			// Render frame
			Draw( m_gameTime->GetDeltaTime() );
		}
	}

	return (int)msg.wParam;
}

void Game::Shutdown()
{
	SAFE_RELEASE( m_rasterizerState );
	SAFE_RELEASE( m_inputLayout );
	SAFE_RELEASE( m_matrixBuffer );
	SAFE_RELEASE( m_vertexShader );
	SAFE_RELEASE( m_pixelShader );

	SAFE_DELETE( m_importReader );
	SAFE_DELETE( m_currentRoom );
	SAFE_DELETE( m_camera );

	for (int i = 0; i < m_rooms.size(); i++)
		SAFE_DELETE( m_rooms.at(i) );

	SAFE_DELETE( m_currentRoom );
	SAFE_DELETE( m_camera );
	SAFE_RELEASE( m_cbLight );

	Application::Shutdown();
}