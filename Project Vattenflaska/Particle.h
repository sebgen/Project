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
			float red, green, blue;
			float velocity;
			bool active;
		};
		struct VertexType
		{
			XMFLOAT3 position;
			XMFLOAT2 texture;
			XMFLOAT4 color;
		};

	private:
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		ID3D11Buffer*			p_vertexBuffer;
		ID3D11Buffer*			p_indexBuffer;
		ID3D11Texture2D*		m_texture;
		ID3D11SamplerState*		m_samplerState;
		ID3D11ShaderResourceView* m_shaderResourceView;
		ID3D11VertexShader*		p_vertexShader;
		ID3D11PixelShader*		p_pixelShader;
		ID3D11InputLayout*		p_inputLayout;
		ID3D11Buffer*			p_matrixBuffer;
		ID3D11BlendState*		p_alphaBlendEnable;
		ID3D11BlendState*		p_alphaBlendDisable;
		Camera*					m_camera;

		XMMATRIX orgVM;
		XMMATRIX orgPM;
		

		float m_particleDevX, m_particleDevY, m_particleDevZ;
		float m_particleVelocity, m_particleVelocityVariation;
		float m_particleSize, m_particlesPerSecond;
		int m_maxParticels;
		int m_currentParticleCount;
		float m_accumulatedTime;
		ParticleType* m_particleList;
		VertexType* m_vertices;
		int m_vertexCount, m_indexCount;

		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		bool InitializeShaders();
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
		Particle(const Particle& other);
		bool init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Camera* cam);
		bool Update(float frameTime);
		void Render();
		ID3D11ShaderResourceView* getTexture();
		int getIndexCount();

};
#endif
