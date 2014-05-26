#ifndef MENU_H
#define	MENU_H

#include "3DLibs.h"
//#include "Application.h"
#include <memory>
//#include "XTK\Inc\SpriteBatch.h"
//#include "XTK\Inc\SpriteFont.h"
//#include "XTK\Inc\DDSTextureLoader.h"



#include "DDSTextureLoader.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"


//#include "../DirectXTK/Inc/DDSTextureLoader.h"
//#include "../DirectXTK/Inc/SpriteBatch.h"
//#include "../DirectXTK/Inc/SpriteFont.h"
//#include "../DirectXTK/Inc/Audio.h"

class Menu
{
private:

	std::unique_ptr<SpriteBatch>spriteBatch;
	std::unique_ptr<SpriteFont> spriteFont;
	
	ID3D11ShaderResourceView*	startMenu;
	ID3D11ShaderResourceView*	pausMenu;
	ID3D11ShaderResourceView*	optionMenu;
	ID3D11ShaderResourceView*	thumbIcon;
	ID3D11ShaderResourceView*	cursor;

	ID3D11DeviceContext*		m_deviceContext;
	ID3D11Device*				m_device;		
	int menuState;
	int thumbState;
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



public:
	Menu();
	~Menu();
	void init(ID3D11DeviceContext* &m_deviceContext, ID3D11Device* &device);
	void Render(int x, int y,float pointX, float pointY, bool m_mouseHit, int fps);
	void setMenuState(int state);
	void setThumbState(int state);
	void shutDown();
	float dist;
};
#endif