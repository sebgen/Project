#ifndef PARTICLE_H
#define	PARTICLE_H
#include "3DLibs.h"
#include "DDSTextureLoader.h"
#include "WVPBuffer.h"
#include "Camera.h"
class Particle
{
	private:
		struct ParticleType
		{
			float posX, posY, posZ;
			float velocity;
			bool active;
			float life;
		};
		struct VertexType
		{
			XMFLOAT3 position;
			XMFLOAT2 texture;
		};

	private:
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		ID3D11Buffer*			p_vertexBuffer;
		ID3D11Buffer*			p_indexBuffer;
		ID3D11Texture2D*		m_texture;
		ID3D11SamplerState*		m_samplerState;
		ID3D11ShaderResourceView* m_shaderResourceView;
		Camera*					m_camera;
		
		XMFLOAT3 m_spread;
		
		

		float m_particleSize;
		float m_lifeTime;
		float m_particleVelocity, m_particleVelocityVariation;
		float m_particlesPerSecond;
		int m_maxParticels;
		int LastUsedParticle;
		float m_accumulatedTime;
		ParticleType* m_particleList;
		VertexType* m_vertices;
		int m_vertexCount, m_indexCount;


		//HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		//bool InitializeShaders();
		bool initParticle();
		bool initBuffers();
		bool updateBuffers();
		bool createTexture(std::wstring filepath);

		void renderBuffers();
		void emitParticle(float frametime);
		void updateParticle(float frametime);
		void killParticles();

	public:
		Particle();
		~Particle();
		XMFLOAT3 m_particlepos;
		Particle(const Particle& other);
		bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Camera* cam, XMFLOAT3 pos, XMFLOAT3 spread, std::wstring filepath);
		bool Update(float frameTime);
		void Render();
		ID3D11ShaderResourceView* getTexture();
		int getIndexCount();
		void shutdown();
		int m_currentParticleCount;
		void setParticlePos(XMFLOAT3 pos);
		void setSpread(XMFLOAT3 spread);
		void setMaxParticles(float maxParticles);
		void setParticleSize(float size);
		void setParticlesPerSecond(float particlePerSecond);
		void setLifeTime(float seconds);

		float testRed;
};
#endif
