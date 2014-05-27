#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "EventSys/EventManagerImpl.h"
#include "EventSys/Events.h"
#include "DrawableObject.h"

enum WheelType{ MusicWheel, BoilerWheel, ButtonWheel };

class Wheel : public DrawableObject
{
	private:
		bool			m_isRotating;
		float			m_rotationTimer;
		std::string		m_name;
		EventManager*	m_em;
		int				m_value;
		WheelType		m_wheelType;
		bool			m_isOn;

	public:
		Wheel();
		Wheel( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo, EventManager* em  );
		~Wheel();

		void RotateWheel();
		int GetValue() const;
		std::string GetName() const;
		void ResetWheel();
		WheelType GetWheelType() const;

		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif