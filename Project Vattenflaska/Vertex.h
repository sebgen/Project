/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description:
 
-------------------------------------------------------------------------
  History:
  2014-04-18 
  Created by Sebastian Genlund
  -----------------------------------------------------------------------
  2014-04-28
  Edited by Simon Johanson
  *Change vertex content from xyz-coordinates to consist of
	-Position
	-Normal
	-Texture coordinates
	-Tangent
*************************************************************************/

#ifndef _VERTEX_H_
#define _VERTEX_H_

struct Vec2
{
	float x;
	float y;
};

struct Vec3
{
	float x;
	float y;
	float z;
};

struct Vec4
{
	float x;
	float y;
	float z;
	float w;
};

struct Vertex
{
	Vec3 position;
	Vec3 normal;
	Vec2 texCoord;
	Vec4 tangent;
};
#endif