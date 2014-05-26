/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Game.h
  
-------------------------------------------------------------------------
  History:
  2014-05-14
  Created by Simon Johanson

*************************************************************************/

#ifndef _GAME_H_
#define _GAME_H_

#include "Application.h"
#include "ImportReader.h"
#include "Camera.h"

class Game : public Application
{
	private:
		//----------
		// DirectX |
		//----------
		ID3D11RasterizerState*	m_rasterizerState;
		ID3D11InputLayout*		m_inputLayout;
		ID3D11Buffer*			m_matrixBuffer;
		WVP_PerFrameBuffer		m_CameraData;
		WVP_PerFrameLightBuffer m_LightData;  /// NY
		ID3D11VertexShader*		m_vertexShader;
		ID3D11PixelShader*		m_pixelShader;


		//-------
		// Game |
		//-------
		ImportReader*		m_importReader;
		std::vector<Room*>	m_rooms;
		Room*				m_currentRoom;
		Camera*				m_camera;

		//------------
		std::vector<GlobalLight> m_lights;

		ID3D11Buffer*			 m_cbLight;  /// NY
		ID3D11Buffer*			 m_cbCamera;


	private:
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		HRESULT InitializeShaders();

		HRESULT CreateCbLightBuffer();  /// NY
		void UpdateCbLight( float deltaTime );  /// NY

		HRESULT CreateCbCameraBuffer();
		void UpdateCbCamera();

		HRESULT Update( float deltaTime );
		HRESULT Draw( float deltaTime );

	public:
		Game();
		~Game();

		HRESULT InitializeGame();
		int Run();
		void Shutdown(); // Release all COM-objects and deallocate member pointers
};
#endif