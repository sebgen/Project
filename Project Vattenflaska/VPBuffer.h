#ifndef _VPBUFFER_H_
#define _VPBUFFER_H_

#include <DirectXMath.h>

struct VPBuffer
{
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};


#endif