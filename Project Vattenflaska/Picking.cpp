#include "Picking.h"

Picking::Picking()
{
	m_windowHeight=0;
	m_windowWidth=0;
	dist=0;
}
Picking::~Picking()
{
}
void Picking::Initialize(int width, int height)
{
	m_windowHeight=height;
	m_windowWidth=width;
}
bool Picking::testIntersectTri(XMFLOAT3 ray_direction, XMFLOAT3 ray_orgin, XMFLOAT3 p0, XMFLOAT3 p1, XMFLOAT3 p2, float& dist)
{
	/*struct Tri
	{
		XMFLOAT3 p0;
		XMFLOAT3 p1;
		XMFLOAT3 p2;
		Tri(XMFLOAT3 p0_, XMFLOAT3 p1_, XMFLOAT3 p2_)
		{
			p0=p0_;
			p1=p1_;
			p2=p2_;
		}
	};
	Tri testTri(p0,p1,p2);*/
	//XMLoadFloat3
	XMVECTOR tempV=XMLoadFloat3(&p1) - XMLoadFloat3(&p0);
	XMFLOAT3 e1;
	XMStoreFloat3(&e1, tempV);

	tempV=XMLoadFloat3(&p2) - XMLoadFloat3(&p0);
	XMFLOAT3 e2;
	XMStoreFloat3(&e2, tempV);

	tempV=XMVector3Cross(XMLoadFloat3(&ray_direction),XMLoadFloat3(&e2));
	XMFLOAT3 q;
	XMStoreFloat3(&q, tempV);

	//XMVECTOR test=XMVector3Dot(XMLoadFloat3(&e1),XMLoadFloat3(&q));
	//a= e1 dot q
	float a=(e1.x*q.x)+(e1.y* q.y)+(e1.z*q.z);

	if(a > -FLT_MIN && a < FLT_MIN)
	{
		return false;
		
	}
	float f= 1/a;
	tempV=XMLoadFloat3(&ray_orgin) - XMLoadFloat3(&p0);
	XMFLOAT3 s;
	XMStoreFloat3(&s, tempV);

	float tempDot=(s.x*q.x)+(s.y*q.y)+(s.z*q.z);
	float u=f*tempDot;

	if(u < 0.0f)
	{
		return false;
		
	}

	tempV=XMVector3Cross(XMLoadFloat3(&s),XMLoadFloat3(&e1));
	XMFLOAT3 r;
	XMStoreFloat3(&r, tempV);

	tempDot=(ray_direction.x * r.x)+(ray_direction.y * r.y)+(ray_direction.z * r.z);
	float v=f*tempDot;

	if(v < 0.0f || u+v > 1.0f)
	{
		return false;
		
	}
	tempDot=(e2.x*r.x)+(e2.y* r.y)+(e2.z * r.z);
	float t=f*tempDot;
	
	if(t > 0.000001)
	{
		dist=t;
		return true;
	}
	return false;
}
bool Picking::testNavMesh(XMFLOAT3 eyepos, std::vector<BoundingBox> info, float& height)
{
	//m_UpDirection( 0.0f, 1.0f, 0.0f ),
	XMFLOAT3 xdir(0.0f, -1.0f, 0.0f);
	XMVECTOR org=XMLoadFloat3(&eyepos);
	XMVECTOR dir=XMLoadFloat3(&xdir);
	for(int i=0; i < info.size(); i++)
	{
		if(info.at(i).Intersects(org, dir, dist))
		{
			height=info.at(i).Center.y;
			return true;
		}
	}
	return false;
	
		/*std::vector<Vertex> vertexinfo=info.vertices;
		int nrOfVert=info.vertexCount;
		XMFLOAT3 p0;
		XMFLOAT3 p1;
		XMFLOAT3 p2;
		
		for(int k=0; k<nrOfVert-3; k+=3)
		{
			p0=XMFLOAT3 (vertexinfo.at(k).position.x, vertexinfo.at(k).position.y, vertexinfo.at(k).position.z);
			p1=XMFLOAT3 (vertexinfo.at(k+1).position.x, vertexinfo.at(k+1).position.y, vertexinfo.at(k+1).position.z);
			p2=XMFLOAT3 (vertexinfo.at(k+2).position.x, vertexinfo.at(k+2).position.y, vertexinfo.at(k+2).position.z);


			if(TriangleTests::Intersects(org, dir, XMLoadFloat3(&p0), XMLoadFloat3(&p1), XMLoadFloat3(&p2), dist))
			{
				int alla=k;
				return true;
			}
			
		}
	
	return false;*/
}
bool Picking::testIntersectBox(int mouseX, int mouseY, XMFLOAT4X4 _projMatrix, XMFLOAT4X4 _viewMatrix,XMFLOAT4X4 _worldMatrix, XMFLOAT3 _orgin)
{
	updateMatrix(_projMatrix, _viewMatrix, _worldMatrix, _orgin);
	testIntersect(mouseX, mouseY);

	XMFLOAT2 corner1(164 ,131);
	XMFLOAT2 corner2(321, 197);

	XMFLOAT2 Max, Min;
	
	float tMax=FLT_MAX;
	float tMin= -FLT_MIN;
	
	Min.x=min(corner1.x, corner2.x);
	Min.y=min(corner1.y, corner2.y);


	Max.x=max(corner1.x, corner2.x);
	Max.y=max(corner1.y, corner2.y);
	
	
	//check x
	if(p_rayDirection.x==0 && p_rayOrgin.x < Min.x && p_rayOrgin.x >Max.x)
	{
		//parallel
		OutputDebugString("paralell x\n");
		return false;
	}
	else
	{
		
		float t1=(Min.x-p_rayOrgin.x)/p_rayDirection.x;
		float t2=(Max.x-p_rayOrgin.x)/p_rayDirection.x;

		if(t1 > t2)
		{
			std::swap(t1,t2);
		}

		if(t1 > tMin)
		{
			tMin=t1;
		}
		if(t2 > tMax)
		{
			tMax=t2;
		}
	//	tMax=tMax;
		if(tMin > tMax)
		{
			OutputDebugString("fail in x tmin >tmax\n");
			return false;

		}
		if(tMax < 0)
		{
			OutputDebugString("fail in x tmax < 0\n");
			return false;
		}
	}
	
	//check y
	if(p_rayDirection.y==0 && p_rayOrgin.y < Min.y && p_rayOrgin.y >Max.y)
	{
		//parallel
		OutputDebugString("paralell y\n");
		return false;
	}
	else
	{
		float t1=(Min.y-p_rayOrgin.y)/p_rayDirection.y;
		float t2=(Max.y-p_rayOrgin.y)/p_rayDirection.y;

		if(t1>t2)
		{
			std::swap(t1,t2);
		}

		if(t1 > tMin)
		{
			tMin=t1;
		}
		if(t2 > tMax)
		{
			tMax=t2;
		}
		if(tMin > tMax)
		{
			OutputDebugString("fail in y tmin >tmax\n");
			return false;
		}
		if(tMax < 0)
		{
			OutputDebugString("fail in y tmax < 0\n");
			return false;
		}
	}

	return true;
}
bool Picking::testIntersectTriXM(int mouseX, int mouseY, XMFLOAT4X4 _projMatrix, XMFLOAT4X4 _viewMatrix,XMFLOAT4X4 _worldMatrix, XMFLOAT3 _orgin, std::vector<MeshInfo> info, int &hitMesh)
{
	updateMatrix(_projMatrix, _viewMatrix, _worldMatrix, _orgin);
	testIntersect(mouseX, mouseY);

	for(int i=0; i<info.size(); i++)
	{
		std::vector<Vertex> vertexinfo=info.at(i).vertices;
		int nrOfVert=info.at(i).vertexCount;
		XMFLOAT3 p0;
		XMFLOAT3 p1;
		XMFLOAT3 p2;
		for(int k=0; k<nrOfVert-3; k+=3)
		{
			p0=XMFLOAT3 (vertexinfo.at(k).position.x, vertexinfo.at(k).position.y, vertexinfo.at(k).position.z);
			p1=XMFLOAT3 (vertexinfo.at(k+1).position.x, vertexinfo.at(k+1).position.y, vertexinfo.at(k+1).position.z);
			p2=XMFLOAT3 (vertexinfo.at(k+2).position.x, vertexinfo.at(k+2).position.y, vertexinfo.at(k+2).position.z);

			/*if(TriangleTests::Intersects(XMLoadFloat3(&p_rayOrgin),XMLoadFloat3(&p_rayDirection),XMLoadFloat3(&p0),XMLoadFloat3(&p1),XMLoadFloat3(&p2),dist))
			{
				testWhatMesh(info.at(i).groupName);
				return true;
			}*/
			if(testIntersectTri(p_rayDirection, p_rayOrgin, p0, p1, p2,dist))
			{
				testWhatMesh(info.at(i).groupName);
				hitMesh=i;
				return true;
			}
		}
	}
	
	return false;
}
void Picking::testWhatMesh(std::string name)
{
	std::string names[4]={"boilerWheelShape1","boilerWheelShape3","boilerWheelShape4","boilerWheelShape5"};
	for(int i=0; i<4; i++)
	{
		if(names[i]==name)
		{
			OutputDebugString(names[i].c_str());
			OutputDebugString("\n");
		}
	}
}
void Picking::testIntersect(int mouseX, int mouseY)

{
	float pointX, pointY;
	XMFLOAT4X4  inverseViewMatrix,  translateMatrix, inverseWorldMatrix;

	XMFLOAT3 direction, rayOrgin, rayDirection;
	bool hr, intersect;

	//flyttar så mouse cordinaterna är innom -1 till +1
	//pointX = ((2.0f*(float)mouseX) / (float)m_windowWidth) - 1.0f;
	//pointY = (((2.0f*(float)mouseY) / (float)m_windowHeight) - 1.0f)*-1;

	pointX =(+2.0f * (float) mouseX / m_windowWidth -1.0f);
	pointY =(-2.0f * (float) mouseY / m_windowHeight +1.0f);
	//ändra points med projMatrix för aspect o viewport
	//projMatrix = m_Camera->getProjection();
	pointX /= projMatrix._11;
	pointY /= projMatrix._22;

	//get inverse viewMatrix
	//viewMatrix = m_Camera->getViewMatrix();
	//XMMATRIX tempM = XMMatrixInverse(&XMMatrixDeterminant(XMLoadFloat4x4(&viewMatrix)), XMLoadFloat4x4(&viewMatrix));
	XMMATRIX tempM = XMMatrixInverse(NULL, XMLoadFloat4x4(&viewMatrix));
	XMStoreFloat4x4(&inverseViewMatrix, tempM);

	//calculate direction of picking ray in viewspace
	direction.x = (pointX* inverseViewMatrix._11) + (pointY* inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX* inverseViewMatrix._12) + (pointY* inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z	= (pointX* inverseViewMatrix._13) + (pointY* inverseViewMatrix._23) + inverseViewMatrix._33;

	//get orgin from camera
	//orgin = m_Camera->getPosition();


	//get worldMatrix and translate to the object
	//worldMatrix = m_Camera->getWorld();
	tempM = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMStoreFloat4x4(&translateMatrix, tempM);

	tempM = XMMatrixMultiply(XMLoadFloat4x4(&worldMatrix), XMLoadFloat4x4(&translateMatrix));
	XMStoreFloat4x4(&worldMatrix, tempM);
	
	//get the inverse of the translateMatrix
	XMStoreFloat4x4(&worldMatrix, tempM);
	//tempM = XMMatrixInverse(&XMMatrixDeterminant(XMLoadFloat4x4(&worldMatrix)), XMLoadFloat4x4(&worldMatrix));
	tempM = XMMatrixInverse(NULL, XMLoadFloat4x4(&worldMatrix));
	XMStoreFloat4x4(&inverseWorldMatrix, tempM);

	//transform rayOrgin and rayDirection from view to world space
	XMVECTOR tempV = XMVector3TransformCoord(XMLoadFloat3(&orgin), XMLoadFloat4x4(&inverseWorldMatrix));
	XMStoreFloat3(&rayOrgin, tempV);

	tempV = XMVector3TransformNormal(XMLoadFloat3(&direction), XMLoadFloat4x4(&inverseWorldMatrix));
	XMStoreFloat3(&rayDirection, tempV);

	//normalize
	tempV = XMVector3Normalize(XMLoadFloat3(&rayDirection));
	XMStoreFloat3(&rayDirection, tempV);

	//sets the ray orgin and ray dir to the ones we gonna use 
	p_rayOrgin=rayOrgin;
	p_rayDirection=rayDirection;
	
}
int Picking::testIntersectMenu(int mouseX, int mouseY, int state)
{
	XMFLOAT2 topright, topleft, botright, botleft;
	

	if(state==MENU)
	{
		//Play
		topright.x=164;
		topright.y=131;

		topleft.x=321;
		topleft.y=131;

		botright.x=164;
		botright.y=197;

		botleft.x=321;
		botleft.y=197;

	
		if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
		{
			OutputDebugString("gonna return start_play\n");
			return START_PLAY;
		}
	
		//option
		topright.x=180;
		topright.y=302;

		topleft.x=402;
		topleft.y=302;

		botright.x=108;
		botright.y=373;

		botleft.x=402;
		botleft.y=373;

		if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
		{
			return START_OPTION;
		}
		//eixt
		topright.x=174;
		topright.y=482;

		topleft.x=309;
		topleft.y=482;

		botright.x=174;
		botright.y=548;

		botleft.x=309;
		botleft.y=548;

		if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
		{
			return START_EXIT;
		}
		
	}

	else if(state==OPTION)
	{
		//mute
		topright.x=147;
		topright.y=184;

		topleft.x=339;
		topleft.y=184;

		botright.x=147;
		botright.y=250;

		botleft.x=339;
		botleft.y=250;

		if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
		{
			return OPTION_MUTE;
		}

		//unmute
		topright.x=110;
		topright.y=315;

		topleft.x=383;
		topleft.y=315;

		botright.x=110;
		botright.y=387;

		botleft.x=383;
		botleft.y=387;

		if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
		{
			return OPTION_UNMUTE;
		}

		//option back
		topright.x=165;
		topright.y=458;

		topleft.x=338;
		topleft.y=458;

		botright.x=165;
		botright.y=524;

		botleft.x=338;
		botleft.y=524;

		if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
		{
			return OPTION_BACK;
		}
	}
	else if(state==PAUS)
	{
			//resume
			topright.x=198;
			topright.y=489;

			topleft.x=414;
			topleft.y=489;

			botright.x=198;
			botright.y=544;

			botleft.x=414;
			botleft.y=544;

			if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
			{
				return PAUS_RESUME;
			}

			//exit
			topright.x=885;
			topright.y=490;

			topleft.x=1012;
			topleft.y=490;

			botright.x=885;
			botright.y=540;

			botleft.x=1012;
			botleft.y=540;

			if(mouseX >topright.x && mouseX < botleft.x && mouseY > topright.y && mouseY < botleft.y)
			{
				return PAUS_EXIT;
			}

	}
	return -1;
}
void Picking::updateMatrix( XMFLOAT4X4 _projMatrix, XMFLOAT4X4 _viewMatrix,XMFLOAT4X4 _worldMatrix, XMFLOAT3 _orgin)
{
	projMatrix=_projMatrix;
	viewMatrix=_viewMatrix;
	worldMatrix=_worldMatrix;
	orgin=_orgin;
}