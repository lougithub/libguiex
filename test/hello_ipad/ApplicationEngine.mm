/*
 *  ApplicationEngine.cpp
 *  libguiex
 *
 *  Created by Lou Guo Liang on 11/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ApplicationEngine.h"

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
	
};

struct IApplicationEngine* CreateApplicationEngine()
{
	return new CLibGuiexEngine;
}

CLibGuiexEngine::CLibGuiexEngine()
{
}

CLibGuiexEngine::~CLibGuiexEngine()
{
}

void CLibGuiexEngine::Initialize( int width, int height, const char* szDataPath )
{
    guiex::CGUIWidgetSystem::Instance()->Initialize();
    guiex::CGUIAssert::SetWarningCB(EditorWarningCB, NULL);
    guiex::CGUIWidgetSystem::Instance()->SetScreenSize(aSize.x,aSize.y);
    guiex::CGUIWidgetSystem::Instance()->SetDataPath(szDataPath);

    //register interface
    GUI_REGISTER_INTERFACE_LIB( "IGUIRender", IGUIRender_opengl);
    GUI_REGISTER_INTERFACE_LIB( "IGUIImageLoader", IGUIImageLoader_tga);
    GUI_REGISTER_INTERFACE_LIB( "IGUIFileSys", IGUIFileSys_stdio);
    //GUI_REGISTER_INTERFACE_LIB( "IGUIMouse", IGUIMouse_winapi);
    //GUI_REGISTER_INTERFACE_LIB( "IGUIFont", IGUIFont_ft2);
    //GUI_REGISTER_INTERFACE_LIB( "IGUIKeyboard", IGUIKeyboard_winapi);
    GUI_REGISTER_INTERFACE_LIB( "IGUIConfigFile", IGUIConfigFile_tinyxml);
    //GUI_REGISTER_INTERFACE_LIB_ARG( "IGUIScript", IGUIScript_lua, FuncInitScript);
    //GUI_REGISTER_INTERFACE_LIB_ARG( "IGUIIme", IGUIIme_winapi, pCanvas->GetHandle());

    //register widget
    guiex::CGUIWidgetGenerator** pGenerator = guiex::GetAllGenerators();
    while(*pGenerator)
    {
        guiex::CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
        pGenerator ++;
    }
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


