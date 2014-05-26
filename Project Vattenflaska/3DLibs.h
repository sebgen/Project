/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: 3DLibs.h
  Collection of libraries, macros and defines
-------------------------------------------------------------------------
  History:
  2014-04-18 
  Created by Sebastian Genlund

*************************************************************************/

#ifndef _3DLIBS_H_
#define _3DLIBS_H_

#include <Windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
//#include <xnamath.h>


#include <vector>
#include <fstream>
#include <string>
#include "Vertex.h"
#include "GlobalLight.h"

using namespace DirectX;

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL;}

#pragma comment( lib, "d3d11.lib")
#pragma comment( lib, "d3dcompiler.lib")



struct MeshInfo
{
	std::string groupName;
	std::string textureName;
	std::string normalMapName;
	std::string specularMapName;

	UINT32 vertexCount;
	std::vector<Vertex> vertices;
};

struct LightInfo
{
	std::string lightName;
	Vec3 diffuse;
	Vec3 position;
	float range;
	float nrOfLights;
};

struct CameraInfo
{
		std::string cameraName;
		Vec3 position;
		Vec3 rotation;
		Vec3 right;
		Vec3 up;
		Vec3 look;
		float nearPlane;
		float farPlane;
};
#endif