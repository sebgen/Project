#include "Scene.h"

HRESULT Scene::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
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

HRESULT Scene::InitializeShaders()
{
	HRESULT hr = S_OK;

	//----------------------
	// Compile Vertex Shader
	//----------------------

	ID3DBlob* vs = nullptr;
	
	if ( SUCCEEDED( hr = CompileShader( "Shaders.sh", "VS_main", "vs_5_0", nullptr, &vs ) ) )
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

	if( SUCCEEDED( hr = CompileShader( "Shaders.sh", "PS_main", "ps_5_0", nullptr, &ps ) ) )
	{
		hr = m_device->CreatePixelShader( ps->GetBufferPointer(),
										  ps->GetBufferSize(),
										  nullptr,
										  &m_pixelShader );
		SAFE_RELEASE( ps );
	}

	return hr;
}

Scene::Scene()
{
	m_importReader		= nullptr;

	m_device			= nullptr;
	m_deviceContext		= nullptr;
	m_rasterizerState	= nullptr;
	m_inputLayout		= nullptr;
	m_camera			= nullptr;
	m_matrixBuffer		= nullptr;

	m_vertexShader		= nullptr;

}

Scene::Scene( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	m_importReader = new ImportReader();

	m_device = device;
	m_deviceContext = deviceContext;

	//------------------------
	// Create Rasterizer State
	//------------------------
	D3D11_RASTERIZER_DESC rd;
	memset( &rd, 0, sizeof( rd ) );
	rd.FillMode			= D3D11_FILL_SOLID; // Solid
	rd.CullMode			= D3D11_CULL_NONE;	// Back
	rd.DepthClipEnable	= true;

	HRESULT hr = m_device->CreateRasterizerState( &rd, &m_rasterizerState );

	m_inputLayout = nullptr;

	// LOAD OBJECT
	m_importReader->LoadObject( m_device, m_deviceContext, m_testObjects, "cave" );
	
	m_camera = new Camera();

	InitializeShaders();

	//---------------------
	// Create Matrix Buffer
	//---------------------
	D3D11_BUFFER_DESC cbd;
	memset( &cbd, 0, sizeof( cbd ) );
	cbd.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage			= D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags  = D3D11_CPU_ACCESS_WRITE;
	cbd.ByteWidth		= sizeof( WVPBuffer );

	hr = m_device->CreateBuffer( &cbd, nullptr, &m_matrixBuffer );
}

Camera* Scene::GetCamera() const
{
	return m_camera;
}
std::vector<TestObject*> Scene::getObjects()const
{
	std::string test="";
	std::vector<TestObject*> objects;
	
	//
	std::string names[4]={"boilerWheelShape1","boilerWheelShape3","boilerWheelShape4","boilerWheelShape5"};
	int namecounter=0;
	for(int i=0; i < m_testObjects.size() && namecounter<4; i++)
	{
		test=m_testObjects.at(i)->getInfo().groupName;
		if(test==names[namecounter])
		{
			namecounter++;
			objects.push_back (m_testObjects.at(i));
		}
	}
	return objects;
}
HRESULT Scene::Update( float deltaTime )
{
	HRESULT hr = S_OK;

	return hr;
}

HRESULT Scene::Draw( float deltaTime )
{
	HRESULT hr = S_OK;
	//D3D11_RASTERIZER_DESC rd;
	//m_rasterizerState->GetDesc(&rd);
	//rd.FillMode=D3D11_FILL_WIREFRAME;
	//m_device->CreateRasterizerState(&rd, &m_rasterizerState);
	//------------
	m_deviceContext->RSSetState( m_rasterizerState );
	
	
	D3D11_MAPPED_SUBRESOURCE ms;
	WVPBuffer* dataPtr;

	m_deviceContext->Map( m_matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms );
	dataPtr = (WVPBuffer*)ms.pData;

	dataPtr->worldMatrix		= DirectX::XMMatrixTranspose( DirectX::XMMatrixIdentity() );
	dataPtr->viewMatrix			= DirectX::XMMatrixTranspose( m_camera->GetViewMatrix() );
	dataPtr->projectionMatrix	= DirectX::XMMatrixTranspose( m_camera->GetProjMatrix() );

	m_deviceContext->Unmap( m_matrixBuffer, NULL );

	m_deviceContext->VSSetConstantBuffers( 0, 1, &m_matrixBuffer );		
	//------------

	// Set topology
	m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	m_deviceContext->IASetInputLayout( m_inputLayout );

	// Set shader stages
	m_deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
	m_deviceContext->HSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->DSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->GSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );

	//TEST
	//---------------------------------------------
	//UINT32 vertexSize	= sizeof( Vertex );
	//UINT32 offset		= 0;

	//ID3D11Buffer** buffersToSet = new ID3D11Buffer*[m_testObjects.size()];
	//for (int i = 0; i < m_testObjects.size(); i++)
	//{
	//	buffersToSet[i] = m_testObjects.at(i).GetVertexBuffer();
	//}

	/*ID3D11Buffer* buffersToSet[] = { m_testObjects.at(0).GetVertexBuffer() };

	m_deviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );*/
	//---------------------------------------------

	// Render Object
	//m_testObjects.at(0)->m_meshInfo
	for (int i = 0; i < m_testObjects.size(); i++)
	{
		m_testObjects.at(i)->Draw( deltaTime );
	}
	return hr;
}