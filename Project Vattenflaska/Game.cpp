#include "Game.h"

HRESULT Game::Update( float deltaTime )
{
	HRESULT hr = S_OK;
	POINT pos= m_input->GetMousePos();

	UpdateCbCamera();
	UpdateCbLight( deltaTime );  /// NY

	if(loadNextLevelNextFrame)
	{
		loadNextLevel();
	}

	if(m_input->GetMouseRButton())
	{
		if(m_checkRClicked==false)
		{
			int whatMeshHit;
			XMFLOAT4X4 world;
			XMMATRIX I=XMMatrixIdentity();
			XMStoreFloat4x4(&world, I);
			m_checkRClicked=true;

			if( m_currentRoom->GetLever().size() > 0 )
			{
				if(m_picker->testIntersectTriXM(pos.x, pos.y, m_camera->GetProjMatrix(), m_camera->GetViewMatrix(), world, m_camera->GetEyePosAsFloat(), m_currentRoom->GetLever(),whatMeshHit))
				{
					m_currentRoom->GetALever( whatMeshHit )->PullLever();

					//====================================================================================
					//										DEBUG
					//====================================================================================
					std::string index = std::to_string( whatMeshHit );
					OutputDebugString( index.c_str() );
					OutputDebugString( (m_currentRoom->GetALever( whatMeshHit )->GetName().c_str() ) );
					OutputDebugString("hit\n");
					std::string ison = std::to_string( m_currentRoom->GetALever( whatMeshHit )->IsOn() );
					OutputDebugString( ison.c_str() );
					OutputDebugString("hit\n");

					ison = std::to_string( m_currentRoom->GetALever( 2 )->IsOn() );
					OutputDebugString("\nLever 1: ");
					OutputDebugString( ison.c_str() );

					ison = std::to_string( m_currentRoom->GetALever( 1 )->IsOn() );
					OutputDebugString("\nLever 2: ");
					OutputDebugString( ison.c_str() );

					ison = std::to_string( m_currentRoom->GetALever( 0 )->IsOn() );
					OutputDebugString("\nLever 3: ");
					OutputDebugString( ison.c_str() );

					ison = std::to_string( m_currentRoom->GetALever( 3 )->IsOn() );
					OutputDebugString("\nLever 4: ");
					OutputDebugString( ison.c_str() );

					ison = std::to_string( m_currentRoom->GetALever( 4 )->IsOn() );
					OutputDebugString("\nLever 5: ");
					OutputDebugString( ison.c_str() );

					//====================================================================================
				}
			}

			if( m_currentRoom->GetWheel().size() > 0 )
			{
				if(m_picker->testIntersectTriXM(pos.x, pos.y, m_camera->GetProjMatrix(), m_camera->GetViewMatrix(), world, m_camera->GetEyePosAsFloat(), m_currentRoom->GetWheel(),whatMeshHit))
				{
					m_currentRoom->GetAWheel( whatMeshHit )->RotateWheel();

					//====================================================================================
					//										DEBUG
					//====================================================================================
					OutputDebugString( "\nfirstMusicWheel: " );
					OutputDebugString( "\nlastMusicWheel: " );

					// Music
					std::string value = std::to_string( m_currentRoom->GetAWheel( 2 )->GetValue() );
					OutputDebugString( "\nMusicWheel 1: " );
					OutputDebugString( value.c_str() );
				
					value = std::to_string( m_currentRoom->GetAWheel( 3 )->GetValue() );
					OutputDebugString( "\nMusicWheel 2: " );
					OutputDebugString( value.c_str() );

					value = std::to_string( m_currentRoom->GetAWheel( 4 )->GetValue() );
					OutputDebugString( "\nMusicWheel 3: " );
					OutputDebugString( value.c_str() );

					value = std::to_string( m_currentRoom->GetAWheel( 5 )->GetValue() );
					OutputDebugString( "\nMusicWheel 4: " );
					OutputDebugString( value.c_str() );

					// Boiler
					value = std::to_string( m_currentRoom->GetAWheel( 6 )->IsOn() );
					OutputDebugString( "\nBoilerWheel 1: " );
					OutputDebugString( value.c_str() );

					value = std::to_string( m_currentRoom->GetAWheel( 7 )->IsOn() );
					OutputDebugString( "\nBoilerWheel 2: " );
					OutputDebugString( value.c_str() );

					value = std::to_string( m_currentRoom->GetAWheel( 8 )->IsOn() );
					OutputDebugString( "\nBoilerWheel 3: " );
					OutputDebugString( value.c_str() );

					value = std::to_string( m_currentRoom->GetAWheel( 9 )->IsOn() );
					OutputDebugString( "\nBoilerWheel 4: " );
					OutputDebugString( value.c_str() );
					//====================================================================================
				}
			}
			if(m_picker->testIntersectTriXM(pos.x, pos.y, m_camera->GetProjMatrix(), m_camera->GetViewMatrix(), world, m_camera->GetEyePosAsFloat(), m_currentRoom->getDoorMesh(),whatMeshHit))
			{
				OutputDebugString("door hit\n");
				loadNextLevelNextFrame=true;
				drawLoadScreen=true;
			}
			else
			{
				//miss
			}
		}
	}
	if(!m_input->GetMouseRButton())
	{
		m_checkRClicked=false;
	}
	if(menuState==PLAY)
	{
		m_currentRoom->Update( deltaTime, m_camera );
	}
	

	handleMovement(deltaTime);
	handleMenu(pos);
	m_input->MouseMove(m_input->GetMouseLButton(),pos.x, pos.y,m_camera);

	return hr;
}
void Game::handleMenu(POINT pos)
{
	if(menuState!=PLAY)
	{
		int thumbprintState=m_picker->testIntersectMenu(pos.x, pos.y, menuState);
		m_menu->setThumbState(thumbprintState);
	}
	if(m_input->GetMouseLButton())
	{
		if(m_checkClicked==false)
		{
			m_checkClicked=true;
			
			int menuChoice=m_picker->testIntersectMenu(pos.x, pos.y, menuState);
			if(menuChoice>-1)
			{
				OutputDebugString("missade inte\n");

				//start menu
				if(menuChoice==START_PLAY)
				{
					OutputDebugString("klickade play\n");
					menuState=PLAY;
					m_menu->setMenuState(PLAY);
					//loadNextLevel();
					
				}
				else if(menuChoice==START_OPTION)
				{
					OutputDebugString("klickade option\n");
					menuState=OPTION;
					m_menu->setMenuState(OPTION);
				}
				else if(menuChoice==START_EXIT)
				{
					OutputDebugString("klickade exit\n");
					exit(0);
				}

				//Option menu
				else if(menuChoice==OPTION_MUTE)
				{
					OutputDebugString("klickade option mute\n");
					//menuState=MENU;
					//m_menu->setState(MENU);
				}
				else if(menuChoice==OPTION_UNMUTE)
				{
					OutputDebugString("klickade option unmute\n");
					//menuState=MENU;
					//m_menu->setState(MENU);
				}
				else if(menuChoice==OPTION_BACK)
				{
					OutputDebugString("klickade option back\n");
					menuState=MENU;
					m_menu->setMenuState(MENU);
				}

				//paus menu
				else if(menuChoice==PAUS_RESUME)
				{
					OutputDebugString("klickade paus resume\n");
					menuState=MENU;
					m_menu->setMenuState(MENU);
				}
				else if(menuChoice==PAUS_EXIT)
				{
					OutputDebugString("klickade paus exit\n");
					exit(0);
					
				}

			}
			
			
		}
		
	}
	if(!m_input->GetMouseLButton())
	{
		m_checkClicked=false;
	}
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
						m_navMesh->moveForward(moveFactor);
						//m_camera->Walk( moveFactor );
						animationTimer = 0.0f;

						if ( moveUnits <= 1  )
						{
							moveUnits = nrOfMovement;
							m_navMesh->moveDone();
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
						//m_camera->Walk( -moveFactor );
						m_navMesh->moveBackWard(moveFactor);
						animationTimer = 0.0f;

						if ( moveUnits <= 1 )
						{
							moveUnits  = nrOfMovement;
							m_navMesh->moveDone();
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

						m_navMesh->moveRight(moveFactor);
						//m_camera->Strafe( moveFactor );
						animationTimer = 0.0f;

						if ( moveUnits <= 1 )
						{
							moveUnits = nrOfMovement;
							m_navMesh->moveDone();
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

						m_navMesh->moveLeft(moveFactor);
						//m_camera->Strafe( -moveFactor );
						animationTimer = 0.0f;

						if ( moveUnits <= 1 )
						{
							moveUnits  = nrOfMovement;
							m_navMesh->moveDone();
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
	if( m_currentRoom->GetRoomName() == "cave2" )
	{
		float blendFactors [] = {0.0f, 0.0f, 0.0f, 0.0f} ;
		m_deviceContext->OMSetBlendState( m_blendState, blendFactors, 0xffffffff );
	}

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

	
	m_currentRoom->Draw( deltaTime );

	



	POINT pos=m_input->GetMousePos();

	float pointX =(+2.0f * (float) pos.x / SCREEN_WIDTH -1.0f);
	float pointY =(-2.0f * (float) pos.y / SCREEN_HEIGHT +1.0f);
	XMMATRIX pm = m_camera->Proj();
	XMFLOAT4X4 projMatrix1;
	XMStoreFloat4x4(&projMatrix1, pm);
	pointX /= projMatrix1._11;
	pointY /= projMatrix1._22;

	m_menu->Render(pos.x, pos.y, pointX, pointY, true, 0);

	if(drawLoadScreen)
	{
		m_menu->DrawLoadScreen();
	}
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

HRESULT Game::CreateBlendState()
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC blendStateDesc = {0} ;
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;

	blendStateDesc.RenderTarget[0].BlendEnable			 = true;
	blendStateDesc.RenderTarget[0].SrcBlend				 = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend			 = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp				 = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha		 = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha		 = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	hr = m_device->CreateBlendState(&blendStateDesc, &m_blendState);

	return hr;
}

Game::Game()
	:Application()
{
	m_rasterizerState	= nullptr;
	m_blendState		= nullptr;
	m_inputLayout		= nullptr;
	m_matrixBuffer		= nullptr;
	m_vertexShader		= nullptr;
	m_pixelShader		= nullptr;

	m_importReader		= nullptr;
	m_currentRoom		= nullptr;
	m_camera			= nullptr;

	m_le				= nullptr;
	m_em				= nullptr;
}

Game::~Game()
{
	delete m_le;
}

HRESULT Game::InitializeGame( EventManager* em )
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
	m_picker=new Picking();
	m_menu= new Menu();
	m_navMesh=new NavMesh();
	InitializeShaders();

	m_picker->Initialize(SCREEN_WIDTH,SCREEN_HEIGHT);
	m_menu->init(m_deviceContext, m_device);
	// Event
	m_em = em;

	// Lua
	m_le = new LuaEngine();
	LuaWrapper::Instance()->Initialize( m_le, m_em );

	//===========================
	// Init Scripts --Dungeon.lua
	//===========================
	LuaWrapper::Instance()->InitDungeonMeta();

	//=================================
	//          LOAD LEVELS          ||
	//=================================
	m_importReader = new ImportReader( m_em );

	//---------------------
	// Load Torture Level |
	//---------------------
	m_importReader->LoadObject( m_device, m_deviceContext, m_rooms, "cave2" ); //"torturelevelfirstdraft" );
	m_currentRoom = m_rooms.at(0);

	CreateCbLightBuffer();  /// NY
	CreateCbCameraBuffer();

	if( m_currentRoom->GetRoomName() == "cave2" )
		CreateBlendState();


	m_navMesh->init(m_picker, m_camera);

	m_importReader->LoadNavMeshObject(m_device, m_deviceContext, m_NavMeshes, "navMeshCave" ); //"navMeshLevel1");
	

	m_navMesh->setMeshInfo(m_NavMeshes.at(1)->getInfo());
	m_navMesh->createTile();
	m_navMesh->setStartPos(m_NavMeshes.at(0)->getInfo());

	return hr;
}

void Game::loadNextLevel()
{
	if(currentLevel==0)
	{
		//load level 0;
		
		OutputDebugString("load martin\n");
		m_currentRoom = m_rooms.at(currentLevel);
		
		currentLevel++;
		drawLoadScreen=false;
		loadNextLevelNextFrame=false;
		return;

	}
	if(currentLevel==1)
	{
		OutputDebugString("load cave\n");
		m_rooms.clear();
		m_importReader->LoadObject( m_device, m_deviceContext, m_rooms, "cave2" );
		m_currentRoom = m_rooms.at(0);
		SAFE_RELEASE(m_cbCamera);
		SAFE_RELEASE(m_cbLight);
		CreateCbLightBuffer();  /// NY
		CreateCbCameraBuffer();

		m_navMesh->clear();
		m_NavMeshes.clear();
		
		m_importReader->LoadNavMeshObject(m_device, m_deviceContext, m_NavMeshes, "navMeshCave");
	

		m_navMesh->setMeshInfo(m_NavMeshes.at(1)->getInfo());
		m_navMesh->createTile();
		m_navMesh->setStartPos(m_NavMeshes.at(0)->getInfo());

		m_camera->RotateY(XMConvertToRadians(90));
		currentLevel++;
		drawLoadScreen=false;
		loadNextLevelNextFrame=false;
		return;
	}
	if(currentLevel==2)
	{
		OutputDebugString("load maze\n");
		m_rooms.clear();
		m_importReader->LoadObject( m_device, m_deviceContext, m_rooms, "maze" );
		m_currentRoom = m_rooms.at(0);
		currentLevel=0;
		drawLoadScreen=false;
		loadNextLevelNextFrame=false;
	}
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

	 m_checkRClicked=false;
	 menuState=MENU;
	 m_checkClicked=false;


	  drawLoadScreen=false;
	 loadNextLevelNextFrame=false;
	//-----------------------------
	 currentLevel=1;
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
			// See GameTime.cpp for implementation
			m_gameTime->Tick();

			// Lock height every frame
			//m_camera->SetHeight( 1.6f );

			HandleInput( msg.message, msg.wParam, msg.lParam );
			// Update game logic
			Update( m_gameTime->GetDeltaTime() );

			// Render frame
			Draw( m_gameTime->GetDeltaTime() );

			// Update events
			m_em->VUpdate();
		}
	}

	return (int)msg.wParam;
}

void Game::Shutdown()
{
	SAFE_RELEASE( m_rasterizerState );
	SAFE_RELEASE( m_blendState );
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