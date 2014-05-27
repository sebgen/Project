#include "NavMesh.h"

NavMesh::NavMesh()
{
}

NavMesh::NavMesh(const NavMesh& other)
{
}

NavMesh::~NavMesh()
{
}
void NavMesh::init(Picking* pick, Camera* cam )
{
	moveSpeed=1;
	height=0.2;
	m_picker=pick;
	m_camera=cam;
	doneMove=true;

	
}
void NavMesh::setMeshInfo(MeshInfo mesh)
{
	info=mesh;
}
void NavMesh::moveForward(float length)
{
	if(doneMove)
	{
		checkWalk(moveSpeed);
	}
	if(!doneMove)
	{
		m_camera->Walk(length);
		m_camera->SetHeight(height);
	}
}
void NavMesh::moveBackWard(float length )
{
	if(doneMove)
	{
		checkWalk(-moveSpeed);
	}
	if(!doneMove)
	{
		m_camera->Walk(-length);
		m_camera->SetHeight(height);
	}
}
void NavMesh::moveLeft(float length )
{
	if(doneMove)
	{
		checkStrafe(-moveSpeed);
	}
	if(!doneMove)
	{
		m_camera->Strafe(-length);
		m_camera->SetHeight(height);
	}
	
}
void NavMesh::moveRight(float length)
{
	if(doneMove)
	{
		checkStrafe(moveSpeed);
	}
	if(!doneMove)
	{
		m_camera->Strafe(length);
		m_camera->SetHeight(height);
	}
}
void NavMesh::checkWalk(float speed)
{
	if(m_picker->testNavMesh(m_camera->fakeWalk(speed), boxes, height))
	{
		doneMove=false;
		//m_camera->Walk(length);

		//m_camera->SetHeight(height);
	}
}
void NavMesh::checkStrafe(float speed)
{
	if(m_picker->testNavMesh(m_camera->fakeStrafe(speed), boxes, height))
	{
		doneMove=false;
	}
}
void NavMesh::moveDone()
{
	doneMove=true;
}
void NavMesh::test(float length)
{

	if(doneMove)
	{
		moveForward(length);
	}
	if(!doneMove)
	{
		m_camera->Walk(length);

		m_camera->SetHeight(height);
	}

}
void NavMesh::setStartPos(MeshInfo pos)
{

	Vertex verts=pos.vertices.at(0);
	XMFLOAT3 p1(verts.position.x,verts.position.y,verts.position.z);
	verts=pos.vertices.at(1);
	XMFLOAT3 p2(verts.position.x,verts.position.y,verts.position.z);

	BoundingBox tempbox;
	BoundingBox::CreateFromPoints(tempbox ,XMLoadFloat3(&p1),XMLoadFloat3(&p2));

	//XMFLOAT3 test3=tempbox.Center;
	m_camera->setCameraPos(tempbox.Center);
	m_camera->SetHeight(0.0f);

}
void NavMesh::createTile()
{
	int nrOfvert=info.vertexCount;
	XMFLOAT3 p1;
	XMFLOAT3 p2;
	Vertex verts;

	for(int i=0; i<nrOfvert-6; i+=6)
	{
		verts=info.vertices.at(i);
		p1=XMFLOAT3(verts.position.x, verts.position.y, verts.position.z);

		verts=info.vertices.at(i+1);
		p2=XMFLOAT3(verts.position.x, verts.position.y, verts.position.z);

		BoundingBox tempbox;
		BoundingBox::CreateFromPoints(tempbox ,XMLoadFloat3(&p1),XMLoadFloat3(&p2));

		boxes.push_back(tempbox);
	}
}

void NavMesh::clear()
{
	boxes.clear();
}