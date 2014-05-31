#include "ImportReader.h"
#include <iostream>

void ImportReader::ImportMeshes( std::fstream &fin )
{
	std::string type;
	int meshCount;

	//Number of meshes to import
	fin >> type >> meshCount;
		
	for (int i = 0; i < meshCount; i++)
	{
		MeshInfo m;
		fin >> type >> m.groupName;	//Name of current mesh	
		GetTextures( m, fin );		//Import current mesh textures
		GetVertices( m, fin );		//Import current mesh vertices
	
		m_meshInfo.push_back(m);	//Add meshinfo to list
	}
}

void ImportReader::ImportCameras( std::fstream &fin )
{
	std::string type;
	int numCams;

	fin >> type >> numCams;

	for (int i = 0; i < numCams; i++)
	{
		CameraInfo c;

		fin >> type >> c.cameraName;
		fin >> type >> c.position.x >> c.position.y >> c.position.z;
		fin >> type >> c.rotation.x >> c.rotation.y >> c.rotation.z;
		fin >> type >> c.right.x >> c.right.y >> c.right.z;
		fin >> type >> c.up.x >> c.up.y >> c.up.z;
		fin >> type >> c.look.x >> c.look.y >> c.look.z;
		fin >> type >> c.nearPlane;
		fin >> type >> c.farPlane;

		m_cameras.push_back(c);
		}
}

void ImportReader::ImportLights( std::fstream &fin )
{
	std::string type;
	int numlights;

	fin >> type >> numlights;

	for (int i = 0; i < numlights; i++)
	{
		LightInfo l;
		fin >> type >> l.lightName;
		fin >> type >> l.diffuse.x >> l.diffuse.y >> l.diffuse.z;
		fin >> type >> l.position.x >> l.position.y >> l.position.z;
		fin >> type >> l.range;
			
		m_lights.push_back(l);
	}
}

void ImportReader::GetVertices( MeshInfo &m, std::fstream &fin )
{
	Vertex v;
	std::string type;

	fin >> type >> m.vertexCount;		//Mesh number of vertices	

	for (int i = 0; i < m.vertexCount; i++)
	{
		fin >> type >> v.position.x >> v.position.y	>> v.position.z;	// Get position of vertex
		fin >> type >> v.normal.x >> v.normal.y	>> v.normal.z;			// Get Normal of vertex
		fin >> type >> v.texCoord.x	>> v.texCoord.y;					//Get UV of vertex
		fin >> type >> v.tangent.x	>> v.tangent.y	>> v.tangent.z;		//Get Tangent of vertex
			
		//add current vertex to list
		m.vertices.push_back(v);
	}
}

void ImportReader::GetTextures( MeshInfo &m, std::fstream &fin )
{
	std::string type;

	//TEXTURE/DIFFUSE
	fin >> type;
	fin.ignore();		//ignore blankspace
	getline(fin, type);	//textureName;

	if( type != "N/A" )
		m.textureName = type;

	//NORMALMAP
	fin >> type;
	fin.ignore();		//ignore blankspace
	getline(fin, type);	//normalName;
	
	if( type != "N/A" )
		m.normalMapName = type;

	//SPECULARMAP
	fin >> type;
	fin.ignore();		//ignore blankspace
	getline(fin, type); //normalName;
	
	if(type != "N/A")
		m.specularMapName = type; 
}

ImportReader::ImportReader( EventManager* em )
{
	m_em = em;
}

ImportReader::~ImportReader()
{}

bool ImportReader::OpenFile( std::string fileName, bool isNavMesh )
{
	std::fstream fin;

	fin.open(fileName + ".txt");
	std::cout << "Opening file: " << fileName << std::endl;
	if (fin.fail() == true)
	{
		std::cout << "ERROR: Couldn't open file: " << fileName << std::endl;
		return false;
	}
	ImportMeshes( fin );
	if(!isNavMesh)
	{
		ImportCameras( fin );
		ImportLights( fin );
	}
	
	
	fin.close();

	return true;
}

bool ImportReader::LoadObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Room*>& rooms, std::string fileName )
{
	// Read file and store all mesh info
	if( !OpenFile( fileName, false) )
		return false;

	//=====================================
	// Create room and fill with content ||
	//=====================================
	Room* tempRoom = new Room( fileName, m_em );

	for (int i = 0; i < m_meshInfo.size(); i++)
	{
		// Add lever to room
		if( m_meshInfo.at(i).groupName.find( "leverShape" ) != std::string::npos )
		{
			Lever* l = new Lever( device, deviceContext, m_meshInfo.at(i), m_em );
			tempRoom->AddLever( l );
		}

		// Add wheel to room
		else if( m_meshInfo.at(i).groupName.find( "heel" ) != std::string::npos )
		{
			Wheel* w = new Wheel( device, deviceContext, m_meshInfo.at(i), m_em );
			tempRoom->AddWheel( w );
		}

		// Add door to room
		else if( m_meshInfo.at(i).groupName.find( "gate" ) != std::string::npos )
		{
			Door* d = new Door( device, deviceContext, m_meshInfo.at(i), m_em );
			tempRoom->AddDoor( d );
		}

		// Add torch to room
		else if( m_meshInfo.at(i).groupName.find( "torch" ) != std::string::npos )
		{
			Torch* t = new Torch( device, deviceContext, m_meshInfo.at(i) );
			tempRoom->AddTorch( t);
		}

		// Add other object to room
		else
		{
			TestObject* t = new TestObject( device, deviceContext, m_meshInfo.at(i) );
			tempRoom->AddContent( t );
		}
	}

	for (int i = 0; i < m_lights.size(); i++)
	{
		tempRoom->AddLight( GlobalLight( XMFLOAT4( m_lights.at(i).diffuse.x, m_lights.at(i).diffuse.y, m_lights.at(i).diffuse.z, 1.0f ),
								 XMFLOAT3( m_lights.at(i).position.x, m_lights.at(i).position.y, m_lights.at(i).position.z ),
								 m_lights.at(i).range ) );
	}


	rooms.push_back( tempRoom );
	//m_lights.clear();
	//m_cameras.clear();
	m_meshInfo.clear();
	return true;
}
bool ImportReader::LoadNavMeshObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<NavMeshObject*>& navMeshObj, std::string fileName )
{
	if( !OpenFile( fileName, true ) )
		return false;

	for (int i = 0; i < m_meshInfo.size(); i++)
	{
		NavMeshObject* N = new NavMeshObject( device, deviceContext );
		N->setObjectData( m_meshInfo.at(i) );
		navMeshObj.push_back( N );
	}
	m_meshInfo.clear();
	return true;
}