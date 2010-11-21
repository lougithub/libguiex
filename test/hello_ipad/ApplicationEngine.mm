/*
 *  ApplicationEngine.cpp
 *  libguiex
 *
 *  Created by Lou Guo Liang on 11/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ApplicationEngine.h"

//libguiex
#include <libguiex_core/guiex.h>
#include <libguiex_widget/guiwgt.h>

//libguiex module
//#include <libguiex_module/keyboard_winapi\guikeyboard_winapi.h>
//#include <libguiex_module/mouse_winapi\guimouse_winapi.h>
//#include <libguiex_module/font_ft2\guifont_ft2.h>
#include <libguiex_module/imageloader_tga/guiimageloader_tga.h>
#include <libguiex_module/filesys_stdio/guifilesys_stdio.h>
#include <libguiex_module/configfile_tinyxml/guiconfigfile_tinyxml.h>
#include <libguiex_module/render_opengles/guirender_opengles.h>
//#include <libguiex_module/script_lua\guiscript_lua.h>
//#include <libguiex_module/ime_winapi\guiime_winapi.h>


class CLibGuiexEngine : public IApplicationEngine
{
public:
	CLibGuiexEngine();
	virtual ~CLibGuiexEngine();
	virtual void Initialize( int width, int height, const char* szDataPath );
	virtual void Update( float deltaTime );
	virtual void OnRotate( DeviceOrientation newOrientation );
	virtual void OnFingerUp(float x, float y);
    virtual void OnFingerDown(float x,float y);
    virtual void OnFingerMove(float oldx, float oldy, float x, float y);
	
private:
	void InitWidgets();
};

struct IApplicationEngine* CreateApplicationEngine()
{
	return new CLibGuiexEngine;
}

void DestroyApplicationEngine( IApplicationEngine* pEngine )
{
	delete pEngine;
}

CLibGuiexEngine::CLibGuiexEngine()
{
    guiex::CGUIWidgetSystem::Instance()->Initialize();	
}

CLibGuiexEngine::~CLibGuiexEngine()
{
	guiex::CGUIWidgetSystem::Instance()->Release();	
}

void CLibGuiexEngine::Initialize( int width, int height, const char* szDataPath )
{
    //guiex::CGUIAssert::SetWarningCB(EditorWarningCB, NULL);
    guiex::CGUIWidgetSystem::Instance()->SetScreenSize(width,height);
    guiex::CGUIWidgetSystem::Instance()->SetDataPath(szDataPath);
    guiex::CGUIWidgetSystem::Instance()->SetDrawExtraInfo(true);
	
    //register interface
    GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengles);
    GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_tga);
    GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_stdio);
    //GUI_REGISTER_INTERFACE_LIB( "IGUIMouse", IGUIMouse_winapi);
    //GUI_REGISTER_INTERFACE_LIB( "IGUIFont", IGUIFont_ft2);
    //GUI_REGISTER_INTERFACE_LIB( "IGUIKeyboard", IGUIKeyboard_winapi);
    GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
    //GUI_REGISTER_INTERFACE_LIB_ARG( "IGUIScript", IGUIScript_lua, FuncInitScript);
    //GUI_REGISTER_INTERFACE_LIB_ARG( "IGUIIme", IGUIIme_winapi, pCanvas->GetHandle());
	
    //register widget
    guiex::CGUIWidgetGenerator** pGenerator = guiex::GetAllGenerators();
    while(*pGenerator)
    {
        guiex::CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
        pGenerator ++;
    }
	
	InitWidgets();
}

void CLibGuiexEngine::InitWidgets()
{	
	if( 0 != guiex::CGUISceneInfoManager::Instance()->LoadScenes())
	{
		return;
	}
	
	for( int i=0; i<guiex::CGUISceneInfoManager::Instance()->GetSceneFilePaths().size(); ++i )
	{
		printf("scene: %s\n", guiex::CGUISceneInfoManager::Instance()->GetSceneFilePaths()[i].c_str());
	}
	
	if( 0 != guiex::CGUISceneUtility::LoadResource( "resource.uip" ))
	{
		return;
	}			   
	
	guiex::CGUIWidget* pWidget_staticimage = GUI_CREATE_WIDGET("CGUIWgtStaticImage", "staticimage_0", "hello_ipad");
	pWidget_staticimage->SetImage( "BGIMAGE", "bg_blue" );
	pWidget_staticimage->SetSize( 200, 200 );
	pWidget_staticimage->SetPosition( 200, 200 );
	pWidget_staticimage->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage->Create();
	
	guiex::CGUIWidget* pWidget_staticimage2 = GUI_CREATE_WIDGET("CGUIWgtStaticImage", "staticimage_2", "hello_ipad");
	pWidget_staticimage2->SetParent( pWidget_staticimage );
	pWidget_staticimage2->SetImage( "BGIMAGE", "bg_blue" );
	pWidget_staticimage2->SetSize( 100, 100 );
	pWidget_staticimage2->SetPosition( 0, 0 );
	pWidget_staticimage2->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage2->SetRotation( 0.0f, 0.0f, 45.0f );
	pWidget_staticimage2->Create();	
	
	{
		guiex::CGUIAsScale* pAsScale = guiex::CGUIAsManager::Instance()->AllocateResourceByTypeChecked<guiex::CGUIAsScale>("CGUIAsScale");
		pAsScale->SetInterpolationValue( guiex::CGUISize(0.1,0.1), guiex::CGUISize(1,1), 1 );
		pAsScale->SetReceiver( pWidget_staticimage );
		//pAsScale->SetLooping( true );
		pAsScale->SetInterpolationType(guiex::eInterpolationType_EaseIn);
		pWidget_staticimage->PlayAs( pAsScale );	
		pAsScale->RefRelease();
	}
	
	{
		guiex::CGUIAsAlpha* pAsAlpha = guiex::CGUIAsManager::Instance()->AllocateResourceByTypeChecked<guiex::CGUIAsAlpha>("CGUIAsAlpha" );
		pAsAlpha->SetInterpolationValue( 0, 1, 1 );
		pAsAlpha->SetReceiver( pWidget_staticimage2 );
		//pAsAlpha->SetLooping( true );
		pAsAlpha->SetInterpolationType(guiex::eInterpolationType_EaseInOut);
		pWidget_staticimage2->PlayAs( pAsAlpha );
		pAsAlpha->RefRelease();
	}
	
	
	guiex::CGUIWidgetSystem::Instance()->AddPage(pWidget_staticimage);
	guiex::CGUIWidgetSystem::Instance()->OpenPage(pWidget_staticimage); 
}



void CLibGuiexEngine::Update( float deltaTime )
{
	guiex::CGUIWidgetSystem::Instance()->Update( deltaTime );
	guiex::CGUIWidgetSystem::Instance()->Render();
}

void CLibGuiexEngine::OnRotate( DeviceOrientation newOrientation )
{
}

void CLibGuiexEngine::OnFingerUp(float x, float y)
{
}

void CLibGuiexEngine::OnFingerDown(float x,float y)
{
}

void CLibGuiexEngine::OnFingerMove(float oldx, float oldy, float x, float y)
{
}


