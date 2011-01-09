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

using namespace guiex;


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
	CGUIFrameworkBase* m_pFramework;
};

extern CGUIFrameworkBase* CreateFramework( );

struct IApplicationEngine* CreateApplicationEngine()
{
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
	m_pFramework = CreateFramework();
	m_pFramework->Initialize( CGUISize( width, height ), (CGUIString(szDataPath) + "/test/").c_str() );
}


void CLibGuiexEngine::Update( float deltaTime )
{
	m_pFramework->Update( deltaTime );
	m_pFramework->Render();
}

void CLibGuiexEngine::OnRotate( DeviceOrientation newOrientation )
{
}

void CLibGuiexEngine::OnFingerUp(float x, float y)
{
	GSystem->ProcessMouseInput( IGUIInterfaceMouse::SMouseEvent( MOUSE_EVENT_UP, MOUSE_LEFT, x, y ) );
}

void CLibGuiexEngine::OnFingerDown(float x,float y)
{
	GSystem->ProcessMouseInput( IGUIInterfaceMouse::SMouseEvent( MOUSE_EVENT_DOWN, MOUSE_LEFT, x, y ) );
}

void CLibGuiexEngine::OnFingerMove(float oldx, float oldy, float x, float y)
{
	GSystem->ProcessMouseInput( IGUIInterfaceMouse::SMouseEvent( MOUSE_EVENT_MOVE, MOUSE_NONE, x, y ) );
}


