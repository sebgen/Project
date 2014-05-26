#ifndef _SCENE_H_
#define _SCENE_H_

#include "ImportReader.h"
#include "Camera.h"
#include "WVPBuffer.h"

class Scene
{
	private:
		ImportReader*			m_importReader;

		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		ID3D11RasterizerState*	m_rasterizerState;
		ID3D11InputLayout*		m_inputLayout;

		std::vector<TestObject*>	m_testObjects;
		Camera*					m_camera;
		ID3D11Buffer*			m_matrixBuffer;
		ID3D11Buffer*			m_lightBuffer;

		ID3D11VertexShader*		m_vertexShader;
		ID3D11PixelShader*		m_pixelShader;

	private:
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		HRESULT InitializeShaders();
	
	public:
		Scene();
		Scene( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
		~Scene();
		std::vector<TestObject*> getObjects()const;
		Camera* GetCamera() const;
		//TestObject* getObject() const;
		HRESULT Update( float deltaTime );
		HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif