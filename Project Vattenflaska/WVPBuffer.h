#ifndef _WVPBUFFER_H_
#define _WVPBUFFER_H_

#include <DirectXMath.h>

struct WVP_PerFrameBuffer
{
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
	DirectX::XMFLOAT4X4 viewProjectionMatrix;
};

struct WVP_PerFrameLightBuffer  /// NY
{
	GlobalLight lights[50];
	DirectX::XMFLOAT4 cameraPosition;
	float nrOfLights;
	float padding[3];

	void clear()
	{
		
	}
};

struct WVP_PerObjectBuffer
{
	DirectX::XMFLOAT4X4 worldMatrix;
};

struct WVP_PerObjectLightBuffer  /// NY
{
	float normalMapEnable;
	float specularMapEnable;
	XMFLOAT2 pad;
};

#endif