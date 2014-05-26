#include "Particle.h"

Particle::Particle()
{
	m_device=nullptr;
	m_deviceContext=nullptr;
	p_vertexBuffer=nullptr;
	p_indexBuffer=nullptr;
	m_texture=nullptr;
	m_samplerState=nullptr;
	m_shaderResourceView=nullptr;
	p_matrixBuffer=nullptr;
	p_alphaBlendDisable=nullptr;
	p_alphaBlendEnable=nullptr;
}
Particle::Particle(const Particle& other)
{
}
Particle::~Particle()
{
}
HRESULT Particle::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
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

bool Particle::InitializeShaders()
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
bool Particle::init(ID3D11Device* device, ID3D11DeviceContext* deviceContest, Camera* cam)
{
	bool result;
	m_device=device;
	m_deviceContext=deviceContest;
	result=createTexture(L"./menuPics/star.dds");
	if(!result)
	{
		return false;
	}
	result=initParticle();
	if(!result)
	{
		return false;
	}
	result=initBuffers();
	if(!result)
	{
		return false;
	}
	HRESULT hr;
	InitializeShaders();
	//fix the constantbuffer
	D3D11_BUFFER_DESC CBbufferdesc;
	memset(&CBbufferdesc, 0, sizeof(CBbufferdesc));
	CBbufferdesc.BindFlags=D3D11_BIND_CONSTANT_BUFFER;
	CBbufferdesc.Usage=D3D11_USAGE_DYNAMIC;
	CBbufferdesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	CBbufferdesc.ByteWidth=sizeof(WVPBuffer);

	if(FAILED(hr=m_device->CreateBuffer(&CBbufferdesc, nullptr, &p_matrixBuffer)))
	{
		return false;
	}
	m_camera=cam;

	orgVM=m_camera->GetViewMatrix();
	orgPM=m_camera->GetProjMatrix();
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


	//blendDesc.RenderTarget[0].BlendEnable = TRUE;
 //   blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].SrcBlendAlpha=D3D11_BLEND_ONE;
	//blendDesc.RenderTarget[0].DestBlend=D3D11_BLEND_INV_SRC_ALPHA;
	//blendDesc.RenderTarget[0].DestBlendAlpha=D3D11_BLEND_INV_SRC_ALPHA;
	//blendDesc.RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].BlendOpAlpha=D3D11_BLEND_OP_ADD;
	//blendDesc.RenderTarget[0].RenderTargetWriteMask=D3D11_COLOR_WRITE_ENABLE_ALL;
	////create the alpha enable state
	//if(FAILED(hr=m_device->CreateBlendState(&blendDesc, &p_alphaBlendEnable)))
	//{
	//	OutputDebugString("failed to create p_alphaBlendEnable\n");
	//	return false;
	//}
	////set the desc to p_alphaBlendEnable
	//blendDesc.RenderTarget[0].BlendEnable=FALSE;
	////create the alpha disable state
	//if(FAILED(hr=m_device->CreateBlendState(&blendDesc, &p_alphaBlendDisable)))
	//{
	//	OutputDebugString("faile to create p_alphaBlendDisable\n");
	//	return false;
	//}
	return true;
}

bool Particle::Update(float frameTime)
{
	
	killParticles();
	emitParticle(frameTime);

	updateParticle(frameTime);

	
	
	return updateBuffers();
}
void Particle::Render()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	//m_deviceContext->OMSetBlendState(p_alphaBlendEnable, nullptr, 0xFFFFFFFF);
	//turn on alpha blend wiht p_alphaBlendEnable
	m_deviceContext->OMSetBlendState(p_alphaBlendEnable, blendFactor, 0xffffffff);
	
	D3D11_MAPPED_SUBRESOURCE ms;
	WVPBuffer* dataPtr;

	m_deviceContext->Map(p_matrixBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	dataPtr=(WVPBuffer*)ms.pData;

	dataPtr->worldMatrix		= DirectX::XMMatrixTranspose( DirectX::XMMatrixIdentity() );
	//dataPtr->viewMatrix			= DirectX::XMMatrixTranspose( orgVM );
	//dataPtr->projectionMatrix	= DirectX::XMMatrixTranspose( orgPM );
	dataPtr->viewMatrix			= DirectX::XMMatrixTranspose( m_camera->GetViewMatrix() );
	dataPtr->projectionMatrix	= DirectX::XMMatrixTranspose( m_camera->GetProjMatrix() );

	m_deviceContext->Unmap(p_matrixBuffer, NULL);

	m_deviceContext->VSSetConstantBuffers(0, 1, &p_matrixBuffer);

	
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_deviceContext->IASetInputLayout(p_inputLayout);

	m_deviceContext->VSSetShader( p_vertexShader, nullptr, 0 );
	m_deviceContext->HSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->DSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->GSSetShader( nullptr, nullptr, 0 );
	m_deviceContext->PSSetShader( p_pixelShader, nullptr, 0 );


	//renderBuffers();
	unsigned int vertexsize=sizeof(VertexType);
	unsigned offset=0;
	//ID3D11Buffer* buffersToSet[] = { p_vertexBuffer };

	m_deviceContext->IASetVertexBuffers(0, 1, &p_vertexBuffer, &vertexsize, &offset);

	m_deviceContext->IASetIndexBuffer(p_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	

	m_deviceContext->PSSetSamplers( 0, 1, &m_samplerState );
	m_deviceContext->PSSetShaderResources(0 ,1, &m_shaderResourceView);
	
	//m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
	m_deviceContext->Draw(m_indexCount,0);
	//turn off alpha blend wiht p_alphaBlendDisable
	m_deviceContext->OMSetBlendState(p_alphaBlendDisable, blendFactor, 0xffffffff);
}

int Particle::getIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* Particle::getTexture()
{
	return m_shaderResourceView;
}

bool Particle::createTexture(std::wstring filepath)
{
	HRESULT hr;
	 if(FAILED(hr=CreateDDSTextureFromFile(m_device, filepath.c_str(),(ID3D11Resource**)& m_texture, nullptr)))
	 {
		 return false;
	 }
	 //set desc
	D3D11_SAMPLER_DESC desc;
	memset(&desc,0, sizeof(desc));
	desc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias		= 0.0f;
	desc.MaxAnisotropy	= 1;
	desc.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	desc.MaxLOD			= D3D11_FLOAT32_MAX;

	if(FAILED(hr=m_device->CreateSamplerState(&desc, &m_samplerState)))
	{
		return false;
	}

	if(FAILED(hr=m_device->CreateShaderResourceView(m_texture, nullptr, &m_shaderResourceView)))
	{
		return false;
	}
	return true;
}
bool Particle::initParticle()
{
	m_particleDevX=0.5f;
	m_particleDevY=0.1f;
	m_particleDevZ=2.0f;

	m_particleVelocity=1.0f;
	m_particleVelocityVariation=0.2f;

	m_particleSize=0.2f;

	m_particlesPerSecond=250.0f;
	
	m_maxParticels=5000;

	// create particle list
	m_particleList=new ParticleType[m_maxParticels];
	if(!m_particleList)
	{
		return false;
	}

	//set all particles to inactive
	for(int i=0; i<m_maxParticels; i++)
	{
		m_particleList[i].active=false;
	}
	// Initialize the current particle count to zero since none are emitted yet.
	m_currentParticleCount = 0;

	// Clear the initial accumulated time for the particle per second emission rate.
	m_accumulatedTime = 0.0f;

	return true;
}

bool Particle::initBuffers()
{
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexDesc;
	D3D11_BUFFER_DESC indexDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT hr;

	//sets the maximum for the vertex array and index array
	m_vertexCount=m_maxParticels*6;
	m_indexCount=m_vertexCount;

	//create the arrays 
	m_vertices=new VertexType[m_vertexCount];
	if(!m_vertices)
	{
		return false;
	}
	
	indices=new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	//init the arrays
	memset(m_vertices, 0, (sizeof(VertexType)*m_vertexCount));

	for(int i=0; i<m_indexCount; i++)
	{
		indices[i]=i;
	}

	//set the vertexDesc
	vertexDesc.Usage=D3D11_USAGE_DYNAMIC;
	vertexDesc.ByteWidth=sizeof(VertexType)*m_vertexCount;
	vertexDesc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	vertexDesc.MiscFlags=0;
	vertexDesc.StructureByteStride=0;

	//set the data
	vertexData.pSysMem=m_vertices;
	vertexData.SysMemPitch=0;
	vertexData.SysMemSlicePitch=0;

	if(FAILED(hr=m_device->CreateBuffer(&vertexDesc, &vertexData, &p_vertexBuffer)))
	{
		return false;
	}

	//set the indexDesc
	indexDesc.Usage=D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth=sizeof(unsigned long)*m_indexCount;
	indexDesc.BindFlags=D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags=0;
	indexDesc.MiscFlags=0;
	indexDesc.StructureByteStride=0;

	//set the data
	indexData.pSysMem=indices;
	indexData.SysMemPitch=0;
	indexData.SysMemSlicePitch=0;

	if(FAILED(hr=m_device->CreateBuffer(&indexDesc, &indexData, &p_indexBuffer)))
	{
		return false;
	}
	//delete the index array cuzwe dont need it anymore
	delete [] indices;

	return true;

}

void Particle::emitParticle(float frameTime)
{
	bool emit, found;
	int index, i, j, posZ;
	
	m_accumulatedTime+=frameTime;

	emit=false;

	//check if its time to emit new particle
	if(m_accumulatedTime > (1.0f/m_particlesPerSecond))
	{
		m_accumulatedTime=0;
		emit=true;
	}

	//if there are particels to emit, emit them emit one per frame
	if(emit && m_currentParticleCount < m_maxParticels-1)
	{
		m_currentParticleCount++;
		

		
		posZ = (((float)rand()-(float)rand())/RAND_MAX) * m_particleDevZ;
		//sort the particles using Z depth
		index=0;
		found=false;
		while(!found)
		{
			if((m_particleList[index].active==false) || (m_particleList[index].posZ < posZ))
			{
				found=true;
			}
			else 
			{
				index++;
			}
		}
		//now put it in the array we use
		i=m_currentParticleCount;
		j=i-1;
		while(i!=index)
		{
			m_particleList[i].posX = m_particleList[j].posX;
			m_particleList[i].posY = m_particleList[j].posY;
			m_particleList[i].posZ = m_particleList[j].posZ;
			m_particleList[i].red = m_particleList[j].red;
			m_particleList[i].green = m_particleList[j].green;
			m_particleList[i].blue = m_particleList[j].blue;
			m_particleList[i].velocity  = m_particleList[j].velocity;
			m_particleList[i].active = m_particleList[j].active;
			i--;
			j--;
		}
		//now randomize new values
			m_particleList[index].posX =(((float)rand()-(float)rand())/RAND_MAX) * m_particleDevX;
			m_particleList[index].posY =(((float)rand()-(float)rand())/RAND_MAX) * m_particleDevY;
			m_particleList[index].posZ = posZ;
			m_particleList[index].red =  (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
			m_particleList[index].green =  (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
			m_particleList[index].blue =  (((float)rand()-(float)rand())/RAND_MAX) + 0.5f;
			m_particleList[index].velocity =  m_particleVelocity + (((float)rand()-(float)rand())/RAND_MAX) * m_particleVelocityVariation;
			m_particleList[index].active = true;
			
	}
	
}
void Particle::updateParticle(float frametime)
{
	for(int i=0; i< m_currentParticleCount; i++)
	{
		m_particleList[i].posY=m_particleList[i].posY-(m_particleList[i].velocity*frametime);
	}
}
void Particle::killParticles()
{
	for(int i=0; i<m_maxParticels; i++)
	{
		if(m_particleList[i].active==true && m_particleList[i].posY < -3.0f)
		{
			m_particleList[i].active=false;
			m_currentParticleCount--;

			for(int j=i; j<m_maxParticels-1; j++)
			{
				m_particleList[j].posX = m_particleList[j+1].posX;
				m_particleList[j].posY = m_particleList[j+1].posY;
				m_particleList[j].posZ = m_particleList[j+1].posZ;
				m_particleList[j].red       = m_particleList[j+1].red;
				m_particleList[j].green     = m_particleList[j+1].green;
				m_particleList[j].blue      = m_particleList[j+1].blue;
				m_particleList[j].velocity  = m_particleList[j+1].velocity;
				m_particleList[j].active    = m_particleList[j+1].active;
			}
		}
	}
}
bool Particle::updateBuffers()
{
	int index;
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE resource;
	VertexType* vertPtr;

	//init the vertex array to zero first
	memset(m_vertices, 0, (sizeof(VertexType)*m_vertexCount));

	//build the vertex arrar form particlelist 
	index=0;
	for(int i=0; i < m_currentParticleCount; i++)
	{
		// Bottom left.
		m_vertices[index].position = XMFLOAT3(m_particleList[i].posX - m_particleSize, m_particleList[i].posY - m_particleSize, m_particleList[i].posZ);
		m_vertices[index].texture = XMFLOAT2(0.0f, 1.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		//Top left
		m_vertices[index].position = XMFLOAT3(m_particleList[i].posX - m_particleSize, m_particleList[i].posY + m_particleSize, m_particleList[i].posZ);
		m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		//Bot right
		m_vertices[index].position = XMFLOAT3(m_particleList[i].posX + m_particleSize, m_particleList[i].posY - m_particleSize, m_particleList[i].posZ);
		m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		//Bot right
		m_vertices[index].position = XMFLOAT3(m_particleList[i].posX - m_particleSize, m_particleList[i].posY - m_particleSize, m_particleList[i].posZ);
		m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;

		//Top left
		m_vertices[index].position = XMFLOAT3(m_particleList[i].posX - m_particleSize, m_particleList[i].posY + m_particleSize, m_particleList[i].posZ);
		m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;
		
		//Top right
		m_vertices[index].position = XMFLOAT3(m_particleList[i].posX + m_particleSize, m_particleList[i].posY + m_particleSize, m_particleList[i].posZ);
		m_vertices[index].texture = XMFLOAT2(1.0f, 0.0f);
		m_vertices[index].color = XMFLOAT4(m_particleList[i].red, m_particleList[i].green, m_particleList[i].blue, 1.0f);
		index++;
	}
	//lock the buffer
	if(FAILED(hr=m_deviceContext->Map(p_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
	{
		OutputDebugString("failed mapping particle buffer\n");
		return false;
	}
	
	vertPtr=(VertexType*)resource.pData;

	//copy the data into the vertex buffer
	memcpy(vertPtr,(void*)m_vertices,(sizeof(VertexType)*m_vertexCount));

	m_deviceContext->Unmap(p_vertexBuffer, 0);


	return true;
}

void Particle::renderBuffers()
{
	unsigned int vertexsize=sizeof(VertexType);
	unsigned offset=0;
	ID3D11Buffer* buffersToSet[] = { p_vertexBuffer };

	m_deviceContext->IASetVertexBuffers(0, 1, buffersToSet, &vertexsize, &offset);

	m_deviceContext->IASetIndexBuffer(p_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}