/** 
* @file game_whackmole.h
* @author ken
* @date 2011-04-14
*/


//============================================================================//
// include
//============================================================================// 
#include "game_whackmole.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}



//*****************************************************************************
//	CMyCanvasLayer_WhackMoleGame
//*****************************************************************************

//------------------------------------------------------------------------------
CMyCanvasLayer_WhackMoleGame::CMyCanvasLayer_WhackMoleGame( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
{
	//set attribute
	SetHitable( true );
	SetMouseConsumed(true);

	CGUIWidget* pPanel = CGUIWidgetManager::Instance()->GetPage( "game_whackmole.xml", "game_whackmole" );
	pPanel->SetParent( this );	
}
//------------------------------------------------------------------------------
CMyCanvasLayer_WhackMoleGame::~CMyCanvasLayer_WhackMoleGame(  )
{
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_WhackMoleGame::OnUpdate(real fDeltaTime)
{
	CGUICanvasLayer::OnUpdate(fDeltaTime );

}
//------------------------------------------------------------------------------
void CMyCanvasLayer_WhackMoleGame::DestroySelf( )
{
	delete this;
}



//*****************************************************************************
//	CGUIFrameworkTest
//*****************************************************************************
//------------------------------------------------------------------------------
CGUIFrameworkTest* CGUIFrameworkTest::ms_pFrameWork = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkTest::CGUIFrameworkTest(  )
:CGUIFramework(  )
,m_pGameLayer(NULL)
{
	ms_pFrameWork = this;
}
//------------------------------------------------------------------------------
CMyCanvasLayer_WhackMoleGame* CGUIFrameworkTest::GetGameLayer()
{
	return m_pGameLayer;
}
//------------------------------------------------------------------------------
int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");

	CGUISceneManager::Instance()->LoadResources( "common" );	
	CGUISceneManager::Instance()->LoadWidgets( "common" );

	CGUISceneManager::Instance()->LoadResources( "game_whackmole" );	
	CGUISceneManager::Instance()->LoadWidgets( "game_whackmole" );

	//add utility widget
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);		

	//create layer
	m_pGameLayer = new CMyCanvasLayer_WhackMoleGame( "turret game layer" );
	m_pGameLayer->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pGameLayer );
	return 0;
}
//------------------------------------------------------------------------------




