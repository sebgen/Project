#ifndef PARTICLEHANDLER_H
#define PARTICLEHANDLER_H

#include "3DLibs.h"
#include "Particle.h"
#include "DDSTextureLoader.h"
#include "VPBuffer.h"
#include "Camera.h"
class ParticleHandler
{
	private:

		std::vector<Particle*> m_particles;
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		ID3D11DepthStencilState* m_stencilStateEnable;
		ID3D11DepthStencilState* m_stencilStateDisable;
		ID3D11VertexShader*		p_vertexShader;
		ID3D11PixelShader*		p_pixelShader;
		ID3D11InputLayout*		p_inputLayout;
		ID3D11Buffer*			p_matrixBuffer;
		ID3D11BlendState*		p_alphaBlendEnable;
		ID3D11BlendState*		p_alphaBlendDisable;
		Camera*					m_camera;
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		bool InitializeShaders();

	public:
		ParticleHandler();
		ParticleHandler(const ParticleHandler& other);
		~ParticleHandler();
		bool Update(float deltatime);
		void Render();
		bool init(ID3D11Device* devince, ID3D11DeviceContext* deviceContex, Camera* cam);
		void addParticleEffect(XMFLOAT3 pos, XMFLOAT3 spread, std::wstring filepath);
		void setParticlePos(int at,XMFLOAT3 pos);
		void setSpread(int at, XMFLOAT3 spread);
		void setMaxParticles(int at, float maxParticles);
		void setParticleSize(int at, float size);
		void setParticlesPerSecond(int at, float particlePerSecond);
		void setLifeTime(int at, float seconds);
		void shutDown();
		void clear();
		float testRed;
};
#endif