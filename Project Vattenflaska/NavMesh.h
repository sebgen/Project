#ifndef NAVMESH_H
#define NAVMESH_H
#include "3DLibs.h"
#include "Picking.h"

#include "Camera.h"
class NavMesh
{
	private:
		float moveSpeed;
		float height;
		DirectX::BoundingBox testbox;
		std::vector<BoundingBox> boxes;
		Picking* m_picker;
		Camera*	m_camera;
		MeshInfo info;
		float lerp(float a, float b, float f);
		void checkWalk(float speed);
		void checkStrafe(float speed);
		bool doneMove;
		
	public:
		NavMesh();
		~NavMesh();
		NavMesh(const NavMesh& other);
		void init(Picking* pick, Camera* cam );
		void setMeshInfo(MeshInfo mesh);
		void moveForward(float length);
		void moveLeft(float length);
		void moveRight(float lengt);
		void moveBackWard(float length);
		void test(float length);
		void createTile();
		void setStartPos(MeshInfo pos);
		void moveDone();
		void clear();
};

#endif