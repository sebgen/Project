#ifndef PICKING_H
#define PICKING_H

#include "3DLibs.h"
#include <DirectXCollision.h>
class Picking
{
private:
	int	m_windowHeight;
	int	m_windowWidth;

	XMFLOAT3 p_rayOrgin;
	XMFLOAT3 p_rayDirection;

	XMFLOAT4X4 projMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 worldMatrix; 
	XMFLOAT3 orgin;

	//menu defines
	//start menu
	//menu defines
	//start menu
	static const int MENU=0;
	static const int PLAY=1;
	static const int START_PLAY=2;
	static const int START_OPTION=3;
	static const int START_EXIT=4;
	//option
	static const int OPTION=5;
	static const int OPTION_MUTE=6;
	static const int OPTION_UNMUTE=7;
	static const int OPTION_BACK=8;
	//paus 
	static const int PAUS=9;
	static const int PAUS_RESUME=10;
	static const int PAUS_EXIT=11;

	void testIntersect(int mouseX, int mouseY);
	void updateMatrix(XMFLOAT4X4 projMatrix, XMFLOAT4X4 viewMatrix,XMFLOAT4X4 worldMatrix, XMFLOAT3 orgin);

public:
	Picking();
	~Picking();
	float dist;
	void Initialize(int width, int height);
	void testWhatMesh(std::string name);
	//bool testIntersectTri(int mouseX, int mouseY, XMFLOAT4X4 projMatrix, XMFLOAT4X4 viewMatrix,XMFLOAT4X4 worldMatrix, XMFLOAT3 orgin);
	bool testIntersectTri(XMFLOAT3 ray_direction, XMFLOAT3 ray_orgin, XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, float& dist);
	bool testIntersectBox(int mouseX, int mouseY, XMFLOAT4X4 projMatrix, XMFLOAT4X4 viewMatrix,XMFLOAT4X4 worldMatrix, XMFLOAT3 orgin);
	int testIntersectMenu(int mouseX, int mouseY, int state);
	bool testIntersectTriXM(int mouseX, int mouseY, XMFLOAT4X4 projMatrix, XMFLOAT4X4 viewMatrix,XMFLOAT4X4 worldMatrix, XMFLOAT3 orgin, std::vector<MeshInfo> info, int &hitMesh);
	bool testNavMesh(XMFLOAT3 eyepos, std::vector<BoundingBox> info, float &height);

};
#endif