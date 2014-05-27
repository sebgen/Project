#ifndef NAVMESHOBJECT_H
#define NAVMESHOBJECT_H

#include "3DLibs.h"

class NavMeshObject
{
	private:
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		MeshInfo				m_meshInfo;
		ID3D11Buffer*			m_vertexBuffer;
		ID3D11Buffer*			m_indexBuffer;
	public:
		NavMeshObject();
		NavMeshObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext );
		~NavMeshObject();

		void setObjectData(MeshInfo info);
		MeshInfo getInfo();

		void shutDown();
};
#endif