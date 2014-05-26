/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: DrawableObject.h
  Pure virtual base class to every object screen

-------------------------------------------------------------------------
  History:
  2014-04-25 
  Created by Simon Johanson

*************************************************************************/

#ifndef _DRAWABLEOBJECT_H_
#define _DRAWABLEOBJECT_H_

#include "3DLibs.h"
#include "DDSTextureLoader.h"
#include "WVPBuffer.h"
#include "Camera.h"

class DrawableObject
{
	public:
		static UINT32	s_nrOfObjects;

	protected:
		ID3D11Device*			 m_device;
		ID3D11DeviceContext*	 m_deviceContext;
		MeshInfo				 m_meshInfo;
		UINT32					 m_objectID;
		ID3D11Buffer*			 m_vertexBuffer;
		ID3D11Buffer*			 m_constantBuffer;
		ID3D11Buffer*			 m_lightInfoBuffer;  /// NY
		WVP_PerObjectBuffer		 m_CBmatrices;
		WVP_PerObjectLightBuffer m_CBLightInfo;  /// NY
		

		// Texture
		ID3D11Texture2D*			m_texture;
		ID3D11SamplerState*			m_samplerStateAnisotropic;
		ID3D11SamplerState*			m_samplerStateLinear;
		ID3D11ShaderResourceView*	m_shaderResourceView;
		ID3D11ShaderResourceView*	m_normalmapRCV;
		ID3D11ShaderResourceView*	m_specularmapRCV;

		// Light
		float m_useNormalMap;
		float m_useSpecularMap;

	private:
		HRESULT CreateTextureAndSamplerState();  /// NY
		HRESULT CreateNormalMap();
		HRESULT CreateSpecularMap();

	protected:
		void UpdateAndSetConstantBuffer();
		void UpdateLightConstantBuffer();  /// NY

	public:
		DrawableObject();
		DrawableObject( const DrawableObject& rhs );
		~DrawableObject();

		ID3D11Buffer* GetVertexBuffer() const;
		std::string GetGroupName() const;
		float GetUseNormalMap() const;
		float GetUseSpecularMap() const;
		XMMATRIX GetWorldMatrix() const;

		void SetObjectData( MeshInfo meshInfo );

		bool operator==( const DrawableObject& rhs );

		virtual HRESULT Update( float deltaTime, Camera* camera ) = 0;
		virtual HRESULT Draw( float deltaTime ) = 0;

		void Shutdown();
};
#endif