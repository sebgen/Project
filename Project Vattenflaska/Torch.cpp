#include "Torch.h"

Torch::Torch()
	:DrawableObject()
{

}

Torch::Torch( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo )
{
	m_device		= device;
	m_deviceContext = deviceContext;
	SetObjectData( meshInfo );
}

Torch::~Torch()
{}

HRESULT Torch::Update( float deltaTime, Camera* camera )
{
	HRESULT hr = S_OK;

	return hr;
}
MeshInfo Torch::getInfo()
{
	return m_meshInfo;
}
HRESULT Torch::Draw( float deltaTime )
{
	HRESULT hr = S_OK;

	UpdateAndSetConstantBuffer();
	UpdateLightConstantBuffer();

	///TEST!
	UINT32 vertexSize	= sizeof( Vertex );
	UINT32 offset		= 0;
	ID3D11Buffer* buffersToSet[] = { m_vertexBuffer };
	m_deviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	//OutputDebugString("sett constatbuffer to torch\n");
	
	m_deviceContext->PSSetSamplers( 0, 1, &m_samplerStateAnisotropic );
	m_deviceContext->PSSetSamplers( 1, 1, &m_samplerStateLinear );

	// Texture
	if( m_meshInfo.textureName.size() != 0 )
		m_deviceContext->PSSetShaderResources( 0, 1, &m_shaderResourceView );

	if( m_meshInfo.normalMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(1, 1, &m_normalmapRCV);

	if( m_meshInfo.specularMapName.size() != 0 )
		m_deviceContext->PSSetShaderResources(2, 1, &m_specularmapRCV);

	m_deviceContext->Draw( m_meshInfo.vertexCount, 0 );
	
	return hr;
}

void Torch::Shutdown()
{
	DrawableObject::Shutdown();
}