/** 
* @file game_td.cpp
* @author ken
* @date 2011-08-15
*/


//============================================================================//
// include
//============================================================================// 
#include "game_td.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 

guiex::CGUIFrameworkBase* GUIEXCreateFramework( )
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
	guiex::CGUICanvasLayer::Initialize();

	guiex::CGUISceneManager::Instance()->LoadResources( "game_td_map_001" );	
	guiex::CGUIWidget* pMapWidget = guiex::CGUISceneManager::Instance()->LoadWidgets( "map_001.xml", "game_td_map_001" );
	pMapWidget->SetParent( this );
	pMapWidget->Open();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::Finalize( )
{
	guiex::CGUICanvasLayer::Finalize();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::OnUpdate(guiex::real fDeltaTime)
{
	CGUICanvasLayer::OnUpdate(fDeltaTime );
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
:guiex::CGUIFramework(  )
,m_pGameLayer(NULL)
{
	ms_pFrameWork = this;
}
//------------------------------------------------------------------------------
guiex::int32 CGUIFrameworkTDGame::InitializeGame()
{
	guiex::CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");

	guiex::CGUISceneManager::Instance()->LoadResources( "common" );	
	guiex::CGUIWidget* pUtilityWidget = guiex::CGUISceneManager::Instance()->LoadWidgets( "utility.xml", "common" );

	//add utility widget
	guiex::GSystem->GetUICanvas()->OpenUIPage(pUtilityWidget);		

	//create game layer
	m_pGameLayer = new CMyCanvasLayer_GameLayer( "td game layer" );
	m_pGameLayer->Initialize();
	guiex::CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pGameLayer );

	return 0;
}
//------------------------------------------------------------------------------



