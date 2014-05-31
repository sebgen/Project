#include "ParticleHandler.h"

ParticleHandler::ParticleHandler()
{
	m_device=nullptr;
	m_deviceContext=nullptr;
	p_matrixBuffer=nullptr;
	p_alphaBlendDisable=nullptr;
	p_alphaBlendEnable=nullptr;
}

ParticleHandler::ParticleHandler(const ParticleHandler& other)
{
}
ParticleHandler::~ParticleHandler()
{

}
HRESULT ParticleHandler::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
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

bool ParticleHandler::InitializeShaders()
{
	HRESULT hr;
	ID3DBlob* vs =nullptr;
	//vertexShader
	if(SUCCEEDED(hr=CompileShader("particleShader.sh", "VS_main", "vs_5_0", nullptr, &vs)))
	{
		if(SUCCEEDED(hr=m_device->CreateVertexShader(vs->GetBufferPointer(),vs->GetBufferSize(),nullptr, &p_vertexShader)))
		{
			D3D11_INPUT_ELEMENT_DESC inputDesc[]={
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			if(FAILED(hr=m_device->CreateInputLayout(inputDesc,ARRAYSIZE(inputDesc),vs->GetBufferPointer(),vs->GetBufferSize(), &p_inputLayout)))
			{
				OutputDebugString("error wiht inputlayout\n");
			}
		}
		
		SAFE_RELEASE(vs);
	}
	else 
	{	
		OutputDebugString("error wiht vs\n");
		return false;
	}
	//pixelShader
	ID3DBlob* ps=nullptr;

	if(SUCCEEDED(hr=CompileShader("particleShader.sh", "PS_main", "ps_5_0", nullptr, &ps)))
	{
		hr=m_device->CreatePixelShader(ps->GetBufferPointer(),ps->GetBufferSize(), nullptr, &p_pixelShader);
		SAFE_RELEASE(ps);

	}
	else
	{
		OutputDebugString("error wiht ps\n");
		return false;
	}
	return true;
}

bool ParticleHandler::init(ID3D11Device* device, ID3D11DeviceContext* deviceContest, Camera* cam)
{
	m_device=device;
	m_deviceContext=deviceContest;
	m_camera=cam;


	HRESULT hr;
	InitializeShaders();
	//fix the constantbuffer
	D3D11_BUFFER_DESC CBbufferdesc;
	memset(&CBbufferdesc, 0, sizeof(CBbufferdesc));
	CBbufferdesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	CBbufferdesc.Usage=D3D11_USAGE_DYNAMIC;
	CBbufferdesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	CBbufferdesc.ByteWidth=sizeof(VPBuffer);

	if(FAILED(hr=m_device->CreateBuffer(&CBbufferdesc, nullptr, &p_matrixBuffer)))
	{
		return false;
	}
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	//Create an alpha enabled blend state description.
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	if(FAILED(hr=m_device->CreateBlendState(&blendDesc, &p_alphaBlendEnable)))
	{
		OutputDebugString("failed to create p_alphaBlendEnable\n");
		return false;
	}
	//set the desc to p_alphaBlendEnable
	blendDesc.RenderTarget[0].BlendEnable=FALSE;
	//create the alpha disable state
	if(FAILED(hr=m_device->CreateBlendState(&blendDesc, &p_alphaBlendDisable)))
	{
		OutputDebugString("faile to create p_alphaBlendDisable\n");
		return false;
	}
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	if(FAILED(hr=m_device->CreateDepthStencilState(&depthStencilDesc, &m_stencilStateEnable)))
	{
		OutputDebugString("error wiht depthStencilDesc\n");
		return false;
	}

	depthStencilDesc.DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ALL;
	if(FAILED(hr=m_device->CreateDepthStencilState(&depthStencilDesc, &m_stencilStateDisable)))
	{
		OutputDebugString("error wiht depthStencilDesc\n");
		return false;
	}
	return true;
}

void ParticleHandler::addParticleEffect(XMFLOAT3 pos, XMFLOAT3 spread,std::wstring filepath)
{
	Particle* tempParticle=new Particle();
	tempParticle->init(m_device, m_deviceContext, m_camera, pos, spread, filepath);
	m_particles.push_back(tempParticle);

}
void ParticleHandler::Render()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	//m_deviceContext->OMSetBlendState(p_alphaBlendEnable, nullptr, 0xFFFFFFFF);
	//turn on alpha blend wiht p_alphaBlendEnable
	m_deviceContext->OMSetDepthStencilState(m_stencilStateEnable, 1);
	m_deviceContext->OMSetBlendState(p_alphaBlendEnable, blendFactor, 0xffffffff);

	

	

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->IASetInputLayout(p_inputLayout);

	m_deviceContext->VSSetShader( p_vertexShader, nullptr, 0 );
	m_deviceContext->HSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->DSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->GSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->PSSetShader( p_pixelShader, nullptr, 0 );

		
	D3D11_MAPPED_SUBRESOURCE ms;
	VPBuffer* dataPtr;
	m_deviceContext->Map(p_matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	dataPtr=(VPBuffer*)ms.pData;

		


	dataPtr->viewMatrix			= DirectX::XMMatrixTranspose( m_camera->View() );
	dataPtr->projectionMatrix	= DirectX::XMMatrixTranspose( m_camera->Proj() );

	m_deviceContext->Unmap(p_matrixBuffer, NULL);
	m_deviceContext->VSSetConstantBuffers(0, 1, &p_matrixBuffer);
	
	for( int i=0; i< m_particles.size(); i++)
	{
		m_particles.at(i)->Render();
	}
	

	m_deviceContext->OMSetBlendState(p_alphaBlendDisable, blendFactor, 0xffffffff);
	m_deviceContext->OMSetDepthStencilState(m_stencilStateDisable, 1);
}

bool ParticleHandler::Update(float deltatime)
{
	for(int i=0; i < m_particles.size(); i++)
	{
		if(m_particles.at(i)->Update(deltatime)==false)
		{
			return false;
		}
	}
	testRed=m_particles.at(0)->testRed;
	return true;
}
void ParticleHandler::setParticlePos(int at, XMFLOAT3 pos)
{
	m_particles.at(at)->setParticlePos(pos);
}
void ParticleHandler::setSpread(int at, XMFLOAT3 spread)
{
	m_particles.at(at)->setSpread(spread);
}
void ParticleHandler::setParticlesPerSecond(int at, float particlePerSecond)
{
	m_particles.at(at)->setParticlesPerSecond(particlePerSecond);
}
void ParticleHandler::setParticleSize(int at, float size)
{
	m_particles.at(at)->setParticleSize(size);
}
void ParticleHandler::setMaxParticles(int at, float maxParticles)
{
	m_particles.at(at)->setMaxParticles(maxParticles);
}
void ParticleHandler::setLifeTime(int at, float seconds)
{
	m_particles.at(at)->setLifeTime(seconds);
}
void ParticleHandler::clear()
{
	for(int i=0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->shutdown();
	}

	m_particles.clear();
}
void ParticleHandler::shutDown()
{
	SAFE_RELEASE(m_stencilStateEnable);
	SAFE_RELEASE(m_stencilStateDisable);
	SAFE_RELEASE(p_vertexShader);
	SAFE_RELEASE(p_pixelShader);
	SAFE_RELEASE(p_inputLayout);
	SAFE_RELEASE(p_matrixBuffer);
	SAFE_RELEASE(p_alphaBlendEnable);
	SAFE_RELEASE(p_alphaBlendDisable);

	for(int i=0; i < m_particles.size(); i++)
	{
		m_particles.at(i)->shutdown();
	}
}


