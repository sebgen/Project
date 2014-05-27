#ifndef _IMPORTREADER_H_
#define _IMPORTREADER_H_

#include "EventSys/EventManagerImpl.h"
#include "EventSys/Events.h"
#include "TestObject.h"
#include "Room.h"
#include "NavMeshObject.h"
#include "Wheel.h"

class ImportReader
{
	private:
		std::vector<MeshInfo>  m_meshInfo;
		std::vector<LightInfo> m_lights;
		std::vector<CameraInfo> m_cameras;
		EventManager*			m_em;

	private:
		void ImportMeshes( std::fstream &fin );
		void ImportCameras( std::fstream &fin );
		void ImportLights( std::fstream &fin );
		void GetVertices( MeshInfo &m, std::fstream &fin );
		void GetTextures( MeshInfo &m, std::fstream &fin );
		bool OpenFile( std::string fileName, bool isNavMesh );

	public:
		ImportReader( EventManager* em );
		~ImportReader();
		
		bool LoadObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Room*>& rooms, std::string fileName );
		bool LoadNavMeshObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<NavMeshObject*>& navMeshObj, std::string fileName );
};
#endif