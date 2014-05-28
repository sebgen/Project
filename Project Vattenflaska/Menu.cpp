#include "Menu.h"

Menu::Menu()
{
	menuState=MENU;
	dist=0;
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
void Menu::Render(int x,int y, float pointX, float pointY, bool m_mouseHit, int fps)
{
	XMFLOAT2 testpos(100,100);
	XMFLOAT2 testpos2(1100,100);
	XMFLOAT2 testpos3(960,100);
	XMFLOAT2 cursorPos(x-50,y-50);
	XMFLOAT2 menupos(0, 0);
	XMFLOAT2 thumbPos(200,100);
	XMFLOAT2 fpsPos(50, 50);
	ID3D11DepthStencilState* currentStencilState=nullptr;
	ID3D11BlendState* currentBlendstate=nullptr;
	UINT blendref=1;
	UINT stencilRef=0;

	m_deviceContext->OMGetDepthStencilState(&currentStencilState, &stencilRef); 

	//m_deviceContext->OMGetBlendState(&currentBlendstate,NULL, &blendref);

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
	
	//XMFLOAT2 crop(59,96);
	//RECT crop;
	//crop.bottom
	if(menuState==MENU)
	{
		if(thumbState==START_PLAY)
		{
			thumbPos.x=100;
			thumbPos.y=120;
			//spriteBatch->Draw(thumbIcon, thumbPos);
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==START_OPTION)
		{
			thumbPos.x=40;
			thumbPos.y=290;
			//spriteBatch->Draw(thumbIcon, thumbPos);
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==START_EXIT)
		{
			thumbPos.x=100;
			thumbPos.y=480;
			//spriteBatch->Draw(thumbIcon, thumbPos);
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
	}
	else if(menuState==OPTION)
	{
		if(thumbState==OPTION_MUTE)
		{
			thumbPos.x=80;
			thumbPos.y=175;
			//spriteBatch->Draw(thumbIcon, thumbPos);
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==OPTION_UNMUTE)
		{
			thumbPos.x=50;
			thumbPos.y=310;
			//spriteBatch->Draw(thumbIcon, thumbPos);
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
		else if(thumbState==OPTION_BACK)
		{
			thumbPos.x=100;
			thumbPos.y=450;
			//spriteBatch->Draw(thumbIcon, thumbPos);
			spriteBatch->Draw(thumbIcon,thumbPos,NULL,Colors::White,0.0f,XMFLOAT2(0,0),0.16f);
		}
	}
	
	
	
	//x=m_input->GetMouseRawX();
	//y=m_input->GetMouseRawY();
	

	
	//ändra points med projMatrix för aspect o viewport
	
	


	std::wstring outputX=(L"X : ");
	std::wstring outputY=(L"Y : ");
	outputX.append(std::to_wstring(x));
	outputY.append(std::to_wstring(y));


	std::wstring outputXIntersect=(L"X : ");
	std::wstring outputYIntersect=(L"Y : ");

	outputXIntersect.append(std::to_wstring(pointX));
	outputYIntersect.append(std::to_wstring(pointY));
	
	//2D värdl pos
	spriteFont->DrawString(spriteBatch.get() ,outputX.c_str(), testpos); 

	testpos.y+=50;
	spriteFont->DrawString(spriteBatch.get() ,outputY.c_str(), testpos); 

	//intersect pos / 3D värd pos 
	spriteFont->DrawString(spriteBatch.get() ,outputXIntersect.c_str(), testpos2); 

	testpos2.y+=50;
	spriteFont->DrawString(spriteBatch.get() ,outputYIntersect.c_str(), testpos2); 

	
	std::wstring outputFps=(L"FPS : ");
	outputFps.append(std::to_wstring(fps));
	spriteFont->DrawString(spriteBatch.get(),outputFps.c_str(),fpsPos);

	if(m_mouseHit)
	{
	
		spriteFont->DrawString(spriteBatch.get() ,L"HIT", testpos3); 
		//m_soundEffect->Play();
	}
	else
	{
		spriteFont->DrawString(spriteBatch.get() ,L"MISS", testpos3); 
	}

	testpos3.y-=50;
	std::wstring distance=std::to_wstring(dist);

	spriteFont->DrawString(spriteBatch.get(), distance.c_str(),testpos3);


	/*float meshX =info.at(0).position.x;
	float meshY=info.at(0).position.y;
	float meshZ=info.at(0).position.z;

	std::wstring meshXOutput=(L"X : ");
	std::wstring meshYOutput=(L"Y : ");
	std::wstring meshZOutput=(L"Z : ");

	meshXOutput.append(std::to_wstring(meshX));
	meshYOutput.append(std::to_wstring(meshY));
	meshZOutput.append(std::to_wstring(meshZ));

	XMFLOAT2 meshPos(700,100);


	spriteFont->DrawString(spriteBatch.get(), meshXOutput.c_str(),meshPos);
	meshPos.y-=50;
	spriteFont->DrawString(spriteBatch.get(), meshYOutput.c_str(),meshPos);
	meshPos.y-=50;
	spriteFont->DrawString(spriteBatch.get(), meshZOutput.c_str(),meshPos);
	*/
	//spriteBatch->Draw(cursor, cursorPos);
	spriteBatch->End();
	//audEng->Update();
	m_deviceContext->OMSetDepthStencilState(currentStencilState, stencilRef);
	//m_deviceContext->OMSetBlendState(currentBlendstate, NULL, 0xffffffff);

	//SAFE_RELEASE(currentBlendstate);
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
}
