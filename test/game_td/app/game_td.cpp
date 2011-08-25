/** 
* @file game_td.cpp
* @author ken
* @date 2011-08-15
*/


//============================================================================//
// include
//============================================================================// 
#include "game_td.h"
#include "tdwgt_game_td.h"
#include <algorithm>

using namespace guiex;
//============================================================================//
// function
//============================================================================// 

CGUIFrameworkBase* GUIEXCreateFramework( )
{
	return new CGUIFrameworkTDGame( );
}

const char* GUIEXGetDataDir()
{
	//should relative to dir "data"
	return "test";
}

//*****************************************************************************
//	CMyCanvasLayer_GameLayer
//*****************************************************************************

//------------------------------------------------------------------------------
CMyCanvasLayer_GameLayer::CMyCanvasLayer_GameLayer( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
,m_pGameWorld(NULL)
{
	//set attribute
	SetHitable( true );
	SetMouseConsumed(true);
}
//------------------------------------------------------------------------------
CMyCanvasLayer_GameLayer::~CMyCanvasLayer_GameLayer(  )
{
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::Initialize( )
{
	CGUICanvasLayer::Initialize();

	//load resource
	CGUISceneManager::Instance()->LoadResources( "game_td_map_001" );	

	//load widget
	CGUIWidget* pMapWidget = CGUISceneManager::Instance()->LoadWidgets( "map_001.xml", "game_td_map_001" );
	pMapWidget->SetParent( this );
	pMapWidget->Open();

	//init game world
	m_pGameWorld = new CTDGameWorld( );
	m_pGameWorld->InitGameWorld( pMapWidget );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::Finalize( )
{
	if( m_pGameWorld )
	{
		m_pGameWorld->DestroyGameWorld();
		delete m_pGameWorld;
		m_pGameWorld = NULL;
	}
	CGUICanvasLayer::Finalize();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------

//
//*****************************************************************************
//	CGUIFrameworkTDGame
//*****************************************************************************
//------------------------------------------------------------------------------
CGUIFrameworkTDGame* CGUIFrameworkTDGame::ms_pFrameWork = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkTDGame::CGUIFrameworkTDGame(  )
:CGUIFramework(  )
,m_pGameLayer(NULL)
{
	ms_pFrameWork = this;
}
//------------------------------------------------------------------------------
int32 CGUIFrameworkTDGame::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");

	CGUISceneManager::Instance()->LoadResources( "common" );	
	CGUIWidget* pUtilityWidget = CGUISceneManager::Instance()->LoadWidgets( "utility.xml", "common" );

	//add utility widget
	GSystem->GetUICanvas()->OpenUIPage(pUtilityWidget);		

	//create game layer
	m_pGameLayer = new CMyCanvasLayer_GameLayer( "td game layer" );
	m_pGameLayer->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pGameLayer );

	return 0;
}
//------------------------------------------------------------------------------
void CGUIFrameworkTDGame::RegisterWidgetGenerators( )
{
	CGUIFramework::RegisterWidgetGenerators();

	//game_td widgets
	{
		CGUIWidgetGenerator** pGenerator = GetAllWidgetGenerators_Game_TD();
		while(*pGenerator)
		{
			CGUIWidgetFactory::Instance()->RegisterGenerator( *pGenerator);
			pGenerator ++;
		}
	}
}
//------------------------------------------------------------------------------
void CGUIFrameworkTDGame::Update( real fDeltaTime )
{
	if( fDeltaTime >= 0.5f )
	{
		fDeltaTime = 0.5f;
	}
	CGUIFramework::Update( fDeltaTime );
}
//------------------------------------------------------------------------------



