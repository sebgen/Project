#include "NavMeshObject.h"

NavMeshObject::NavMeshObject()
{
	m_device				= nullptr;
	m_deviceContext			= nullptr;
	m_vertexBuffer			= nullptr;
	m_indexBuffer			= nullptr;
}

NavMeshObject::NavMeshObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	m_device = device;
	m_deviceContext = deviceContext;
}
NavMeshObject::~NavMeshObject()
{
}
void NavMeshObject::setObjectData(MeshInfo info)
{
	m_meshInfo.groupName = info.groupName;
	m_meshInfo.normalMapName = info.normalMapName;
	m_meshInfo.textureName = info.textureName;
	m_meshInfo.vertexCount = info.vertexCount;
	m_meshInfo.vertices = info.vertices;

	D3D11_BUFFER_DESC desc;
	desc.ByteWidth=sizeof(Vertex)*m_meshInfo.vertexCount;
	desc.StructureByteStride=sizeof(Vertex);
	desc.Usage=D3D11_USAGE_DEFAULT;
	desc.BindFlags=D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags=0;
	desc.MiscFlags=0;

	D3D11_SUBRESOURCE_DATA data;
	data.SysMemPitch=0;
	data.SysMemSlicePitch=0;
	data.pSysMem=&m_meshInfo.vertices[0];

	HRESULT hr;
	if(FAILED(hr=m_device->CreateBuffer(&desc, &data, &m_vertexBuffer)))
	{
		OutputDebugString("failed to create navMeshObject buffer\n");
	}

}

MeshInfo NavMeshObject::getInfo()
{
	return m_meshInfo;
}

void NavMeshObject::shutDown()
{
	SAFE_RELEASE( m_vertexBuffer );
	SAFE_RELEASE( m_indexBuffer );
}