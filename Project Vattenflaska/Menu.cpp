#include "Menu.h"

Menu::Menu()
{
	menuState=MENU;
}
Menu::~Menu()
{
	
}
void Menu::setMenuState(int state)
{
	menuState=state;
}
void Menu::setThumbState(int state)
{
	thumbState=state;
}
void Menu::DrawLoadScreen()
{
	ID3D11DepthStencilState* currentStencilState=nullptr;
	ID3D11BlendState* currentBlendstate=nullptr;
	UINT blendref=1;
	UINT stencilRef=0;

	m_deviceContext->OMGetDepthStencilState(&currentStencilState, &stencilRef); 

	m_deviceContext->OMGetBlendState(&currentBlendstate,NULL, &blendref);
	
	spriteBatch->Begin();
	
	spriteBatch->Draw(loadscreen, XMFLOAT2 (0, 0));

	spriteBatch->End();

	m_deviceContext->OMSetDepthStencilState(currentStencilState, stencilRef);
	m_deviceContext->OMSetBlendState(currentBlendstate, NULL, 0xffffffff);

	SAFE_RELEASE(currentBlendstate);
	SAFE_RELEASE(currentStencilState);

}
void Menu::DrawLetter()
{
	ID3D11DepthStencilState* currentStencilState=nullptr;
	//ID3D11BlendState* currentBlendstate=nullptr;
	UINT blendref=1;
	UINT stencilRef=0;

	m_deviceContext->OMGetDepthStencilState(&currentStencilState, &stencilRef); 

	//m_deviceContext->OMGetBlendState(&currentBlendstate,NULL, &blendref);
	
	spriteBatch->Begin();
	
	spriteBatch->Draw(letter, XMFLOAT2 (0, 0));

	spriteBatch->End();

	m_deviceContext->OMSetDepthStencilState(currentStencilState, stencilRef);
	//m_deviceContext->OMSetBlendState(currentBlendstate, NULL, 0xffffffff);

	//SAFE_RELEASE(currentBlendstate);
	SAFE_RELEASE(currentStencilState);

}
void Menu::DrawEndScreen()
{
	ID3D11DepthStencilState* currentStencilState=nullptr;
	ID3D11BlendState* currentBlendstate=nullptr;
	UINT blendref=1;
	UINT stencilRef=0;

	m_deviceContext->OMGetDepthStencilState(&currentStencilState, &stencilRef); 

	m_deviceContext->OMGetBlendState(&currentBlendstate,NULL, &blendref);
	
	spriteBatch->Begin();
	
	spriteBatch->Draw(EndScreen, XMFLOAT2 (0, 0));

	spriteBatch->End();

	m_deviceContext->OMSetDepthStencilState(currentStencilState, stencilRef);
	m_deviceContext->OMSetBlendState(currentBlendstate, NULL, 0xffffffff);

	SAFE_RELEASE(currentBlendstate);
	SAFE_RELEASE(currentStencilState);

}
void Menu::Render()
{
	XMFLOAT2 menupos(0, 0);
	XMFLOAT2 thumbPos(200,100);
	ID3D11DepthStencilState* currentStencilState=nullptr;
	ID3D11BlendState* currentBlendstate=nullptr;
	UINT blendref=1;
	UINT stencilRef=0;

	m_deviceContext->OMGetDepthStencilState(&currentStencilState, &stencilRef); 

	m_deviceContext->OMGetBlendState(&currentBlendstate,NULL, &blendref);

	spriteBatch->Begin();

	
	
	
	if(menuState==MENU)
	{
		spriteBatch->Draw(startMenu, menupos);
	}

	else if(menuState==OPTION)
	{
		spriteBatch->Draw(optionMenu, menupos);
	}
	else if(menuState==PAUS)
	{
		spriteBatch->Draw(pausMenu, menupos);
	}
	else if(menuState==ENDSCREEN)
	{
		spriteBatch->Draw(EndScreen, menupos);
	}
	
	if(menuState==MENU)
	{
		if(thumbState==START_PLAY)
		{
			thumbPos.x=180;//100;
			thumbPos.y=210;//120;
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==START_OPTION)
		{
			thumbPos.x=90;
			thumbPos.y=470;
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==START_EXIT)
		{
			thumbPos.x=100+80;
			thumbPos.y=730;
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
	}
	else if(menuState==OPTION)
	{
		if(thumbState==OPTION_MUTE)
		{
			thumbPos.x=160;
			thumbPos.y=290;
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==OPTION_UNMUTE)
		{
			thumbPos.x=100;
			thumbPos.y=490;
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==OPTION_BACK)
		{
			thumbPos.x=180;
			thumbPos.y=700;
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
	}
	
	spriteBatch->End();
	//audEng->Update();
	m_deviceContext->OMSetDepthStencilState(currentStencilState, stencilRef);
	m_deviceContext->OMSetBlendState(currentBlendstate, NULL, 0xffffffff);

	SAFE_RELEASE(currentBlendstate);
	SAFE_RELEASE(currentStencilState);
	
}
void Menu::init(ID3D11DeviceContext* &deviceContext, ID3D11Device* &device)
{

	m_deviceContext=deviceContext;
	m_device=device;
	spriteFont.reset(new SpriteFont(m_device,L"./Arial.spritefont"));

	spriteBatch.reset(new SpriteBatch(m_deviceContext));

	CreateDDSTextureFromFile(m_device, L"./menuPics/startmenu.dds", nullptr, &startMenu);
	CreateDDSTextureFromFile(m_device, L"./menuPics/pausmenu.dds", nullptr, &pausMenu);
	CreateDDSTextureFromFile(m_device, L"./menuPics/optionsmenu.dds", nullptr, &optionMenu);
	CreateDDSTextureFromFile(m_device, L"./menuPics/thumbprint.dds", nullptr, &thumbIcon);
	CreateDDSTextureFromFile(m_device, L"./menuPics/star.dds", nullptr, &cursor);
	CreateDDSTextureFromFile(m_device, L"./menuPics/loadingscreen.dds", nullptr, &loadscreen);
	CreateDDSTextureFromFile(m_device, L"./menuPics/Endmenu.dds", nullptr, &EndScreen);
	CreateDDSTextureFromFile(m_device, L"./menuPics/letter.dds", nullptr, &letter);
	//size_t size=0U;
	//DDS_ALPHA_MODE *alpha= (DDS_ALPHA_MODE*)DDS_ALPHA_MODE_STRAIGHT;
	//CreateDDSTextureFromFile(m_device, L"./menuPics/thumbprint.dds", nullptr, &thumbIcon,size ,alpha);
	//CoInitializeEx( nullptr, COINIT_MULTITHREADED );
	   // Create DirectXTK for Audio objects
}
void Menu::shutDown()
{
	SAFE_RELEASE(startMenu);
	SAFE_RELEASE(pausMenu);
	SAFE_RELEASE(optionMenu);
	SAFE_RELEASE(thumbIcon);
	SAFE_RELEASE(cursor);
	SAFE_RELEASE(letter);
	SAFE_RELEASE(EndScreen);
}
