#ifndef _IMPORTREADER_H_
#define _IMPORTREADER_H_

#include "TestObject.h"
#include "Room.h"


class ImportReader
{
	private:
		std::vector<MeshInfo>  m_meshInfo;
		std::vector<LightInfo> m_lights;
		std::vector<CameraInfo> m_cameras;

	private:
		void ImportMeshes( std::fstream &fin );
		void ImportCameras( std::fstream &fin );
		void ImportLights( std::fstream &fin );
		void GetVertices( MeshInfo &m, std::fstream &fin );
		void GetTextures( MeshInfo &m, std::fstream &fin );
		bool OpenFile( std::string fileName );

	public:
		ImportReader();
		~ImportReader();
		
		bool LoadObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Room*>& rooms, std::string fileName );
};
#endif