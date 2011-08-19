/*
 *  common_ios_engine.cpp
 *
 *  Created by Lou Guo Liang on 11/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "common_ios_engine.h"

//libguiex
#include <libguiex_framework/guiframework.h>
#include <libguiex_core/guiinterfacemouse.h>
#include <time.h>

using namespace guiex;


class CLibGuiexEngine : public IApplicationEngine
{
public:
	CLibGuiexEngine();
	virtual ~CLibGuiexEngine();
	virtual void Initialize( int width, int height, const char* szDataPath );
	virtual void Update( float deltaTime );
	virtual void OnRotate( EDeviceOrientation newOrientation );
	virtual void OnFingerUp(float x, float y);
    virtual void OnFingerDown(float x,float y);
    virtual void OnFingerMove(float oldx, float oldy, float x, float y);
	
private:
	CGUIFrameworkBase* m_pFramework;
};

extern CGUIFrameworkBase* GUIEXCreateFramework( );
extern const char* GUIEXGetDataDir();

struct IApplicationEngine* CreateApplicationEngine()
{
	srand(time(NULL));
	return new CLibGuiexEngine;
}

void DestroyApplicationEngine( IApplicationEngine* pEngine )
{
	delete pEngine;
}

CLibGuiexEngine::CLibGuiexEngine()
	:m_pFramework( NULL )
{
}

CLibGuiexEngine::~CLibGuiexEngine()
{
	if( m_pFramework )
	{
		m_pFramework->Release();
		delete m_pFramework;
		m_pFramework = NULL;
	}
}

void CLibGuiexEngine::Initialize( int width, int height, const char* szDataPath )
{
	m_pFramework = GUIEXCreateFramework();
	m_pFramework->Initialize( CGUIIntSize( width, height ), (CGUIString(szDataPath) + "/" + GUIEXGetDataDir()).c_str() );

    OnRotate(eDeviceOrientation_Portrait);
}


void CLibGuiexEngine::Update( float deltaTime )
{
	m_pFramework->Update( deltaTime );
	m_pFramework->Render();
}

void CLibGuiexEngine::OnRotate( EDeviceOrientation newOrientation )
{
	if( GSystem->IsFixedScreenOrientation() )
	{
		return;
	}

	switch( newOrientation )
	{
		case eDeviceOrientation_Portrait:
			GSystem->SetScreenOrientation( eScreenOrientation_Portrait );
			break;
		case eDeviceOrientation_PortraitUpsideDown:
			GSystem->SetScreenOrientation( eScreenOrientation_PortraitUpsideDown );
			break;
		case eDeviceOrientation_LandscapeLeft:
			GSystem->SetScreenOrientation( eScreenOrientation_LandscapeLeft );
			break;
		case eDeviceOrientation_LandscapeRight:
			GSystem->SetScreenOrientation( eScreenOrientation_LandscapeRight );
			break;
	}
}

void CLibGuiexEngine::OnFingerUp(float x, float y)
{
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(CGUIVector2(x,y));
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_LEFT, MOUSE_UP );
}

void CLibGuiexEngine::OnFingerDown(float x,float y)
{
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(CGUIVector2(x,y));	
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeButtonState( MOUSE_LEFT, MOUSE_DOWN );
}

void CLibGuiexEngine::OnFingerMove(float oldx, float oldy, float x, float y)
{
	CGUIInterfaceManager::Instance()->GetInterfaceMouse()->ChangeMousePos(CGUIVector2(x,y));
}


