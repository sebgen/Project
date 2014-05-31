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
}
Particle::Particle(const Particle& other)
{
}
Particle::~Particle()
{
}

bool Particle::init(ID3D11Device* device, ID3D11DeviceContext* deviceContest, Camera* cam, XMFLOAT3 pos, XMFLOAT3 spread, std::wstring filepath)
{
	bool result;
	m_device=device;
	m_deviceContext=deviceContest;
	m_camera=cam;
	m_particlepos=pos;
	m_spread=spread;
	result=createTexture(filepath.c_str());
	



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

	return true;
}
void Particle::setParticlePos(XMFLOAT3 pos)
{
	m_particlepos=pos;
}
void Particle::setSpread(XMFLOAT3 spread)
{
	m_spread=spread;
}
void Particle::setParticlesPerSecond(float particlePerSecond)
{
	m_particlesPerSecond=particlePerSecond;
}
void Particle::setParticleSize(float size)
{
	m_particleSize=size;
}
void Particle::setMaxParticles(float maxParticles)
{
	m_maxParticels=maxParticles;
}
void Particle::setLifeTime(float seconds)
{
	m_lifeTime=seconds;
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
	


	unsigned int vertexsize=sizeof(VertexType);
	unsigned offset=0;
	ID3D11Buffer* buffersToSet[] = { p_vertexBuffer };

	m_deviceContext->IASetVertexBuffers(0, 1, &p_vertexBuffer, &vertexsize, &offset);

	m_deviceContext->IASetIndexBuffer(p_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_deviceContext->PSSetSamplers( 0, 1, &m_samplerState );
	m_deviceContext->PSSetShaderResources(0 ,1, &m_shaderResourceView);
	
	m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
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
	m_lifeTime=1.0f;

	m_particleVelocity=0.5f;
	m_particleVelocityVariation=0.2f;

	m_particleSize=0.05f;

	m_particlesPerSecond=50;
	
	m_maxParticels=50;

	// create particle list
	m_particleList=new ParticleType[m_maxParticels];
	if(!m_particleList)
	{
		return false;
	}

	//set all particles to inactive
	for(int i=0; i<m_maxParticels; i++)
	{
		m_particleList[i].active=true;
		m_particleList[i].life=m_lifeTime;
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
	int index, i, j;
	float posZ;
	
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
		

		
		posZ = (((float)rand()-(float)rand())/RAND_MAX) * m_spread.z;
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
			m_particleList[i].velocity  = m_particleList[j].velocity;
			m_particleList[i].active = m_particleList[j].active;
			m_particleList[i].life = m_particleList[j].life;
			i--;
			j--;
		}
		//now randomize new values
		
		m_particleList[index].posX =(((float)rand()-(float)rand())/RAND_MAX) * m_spread.x;
		m_particleList[index].posX+=m_particlepos.x;

		m_particleList[index].posY =(((float)rand()-(float)rand())/RAND_MAX) * m_spread.y;
		m_particleList[index].posY+=m_particlepos.y;

		m_particleList[index].posZ = posZ;
		m_particleList[index].posZ+=m_particlepos.z;
		

		m_particleList[index].velocity =  m_particleVelocity + (((float)rand()-(float)rand())/RAND_MAX) * m_particleVelocityVariation;
		m_particleList[index].active = true;
		m_particleList[index].life=m_lifeTime;
			
	}
	
}
void Particle::updateParticle(float frametime)
{
	
	for(int i=0; i< m_currentParticleCount; i++)
	{
		m_particleList[i].life-=frametime;
		m_particleList[i].posY+=(m_particleList[i].velocity*frametime);
	}
}
void Particle::killParticles()
{
	for(int i=0; i<m_maxParticels; i++)
	{
		if(m_particleList[i].active==true && m_particleList[i].life < 0.0f)
		{
			m_particleList[i].active=false;
			//m_particleList[i].posX=particlepos.x;
			//m_particleList[i].life=5.0f;
			m_currentParticleCount--;


			for(int j=i; j<m_maxParticels-1; j++)
			{
				m_particleList[j].posX = m_particleList[j+1].posX;
				m_particleList[j].posY = m_particleList[j+1].posY;
				m_particleList[j].posZ = m_particleList[j+1].posZ;
				m_particleList[j].velocity  = m_particleList[j+1].velocity;
				m_particleList[j].active    = m_particleList[j+1].active;
				m_particleList[j].life		= m_particleList[j+1].life;
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
	float scale=0.05f;
	XMVECTOR U= XMLoadFloat3(&XMFLOAT3(0, 1, 0));
	XMFLOAT3 finalpos;

	for(int i=0; i < m_currentParticleCount; i++)
	{
		
		XMVECTOR P= XMLoadFloat3(&XMFLOAT3(m_particleList[i].posX, m_particleList[i].posY, m_particleList[i].posZ));

		XMVECTOR F=m_camera->GetEyePos() - P;
		F=XMVector3Normalize(F);
		XMVECTOR L=XMVector3Cross(F, U);
		L=XMVector3Normalize(L);

		
		XMVECTOR V0=P-((L+U)*scale);
		XMVECTOR V1=P+((L+U)*scale);
		XMVECTOR V2=P+((L-U)*scale);
		XMVECTOR V3=P-((L-U)*scale);
		


		XMStoreFloat3(&finalpos, V3);
		// Bottom left.
		//m_vertices[index].position = /*finalpos;*/XMFLOAT3(m_particleList[i].posX - m_particleSize, m_particleList[i].posY - m_particleSize, m_particleList[i].posZ);
		m_vertices[index].position=XMFLOAT3(finalpos.x, m_particleList[i].posY-m_particleSize, finalpos.z);
		m_vertices[index].texture = XMFLOAT2(0.0f, 1.0f);
		index++;

		XMStoreFloat3(&finalpos, V0);
		//Top left
		//m_vertices[index].position = /*finalpos;*/XMFLOAT3(m_particleList[i].posX - m_particleSize, m_particleList[i].posY + m_particleSize, m_particleList[i].posZ);
		m_vertices[index].position=XMFLOAT3(finalpos.x, m_particleList[i].posY+ m_particleSize, finalpos.z);
		m_vertices[index].texture = XMFLOAT2(0.0f, 0.0f);
		index++;

		XMStoreFloat3(&finalpos, V2);
		//Bot right
		//m_vertices[index].position = /*finalpos;*/XMFLOAT3(m_particleList[i].posX + m_particleSize, m_particleList[i].posY - m_particleSize, m_particleList[i].posZ);
		m_vertices[index].position=XMFLOAT3(finalpos.x, m_particleList[i].posY- m_particleSize, finalpos.z);
		m_vertices[index].texture = XMFLOAT2(1.0f, 1.0f);
		index++;

		
		//Bot right
		m_vertices[index]=m_vertices[index-1];
		index++;

		//topleft
		m_vertices[index]=m_vertices[index-3];
		index++;
		
		XMStoreFloat3(&finalpos, V1);
		//Top right
		//m_vertices[index].position = /*finalpos;*/ XMFLOAT3(m_particleList[i].posX + m_particleSize, m_particleList[i].posY + m_particleSize, m_particleList[i].posZ);
		m_vertices[index].position=XMFLOAT3(finalpos.x, m_particleList[i].posY + m_particleSize, finalpos.z);
		m_vertices[index].texture = XMFLOAT2(1.0f, 0.0f);
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

void Particle::shutdown()
{
	SAFE_RELEASE( p_vertexBuffer );
	SAFE_RELEASE( p_indexBuffer );


	SAFE_RELEASE( m_texture );
	SAFE_RELEASE( m_samplerState );
	SAFE_RELEASE( m_shaderResourceView );
}