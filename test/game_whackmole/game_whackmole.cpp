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
//	CMyMole
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyMole );
//------------------------------------------------------------------------------
CMyMole::CMyMole( const CGUIString& rName, const CGUIString& rSceneName )
:CGUIWgtAnimation( StaticGetType(), rName, rSceneName )
,m_eMoleState( eMoleState_Hide )
,m_pAs(NULL)
{

}
//------------------------------------------------------------------------------
void CMyMole::InitMole( CGUIWgtTag* pTag )
{
	//enable mouse hit
	SetHitable( true );

	//set parent
	pTag->GetParent()->InsertChild( pTag, this );

	SetAnimation( "normal", "mole" );
	SetAnimation( "laugh", "mole_laugh" );
	SetAnimation( "thump", "mole_thump" );

	SetCurrentAnimation( "normal" );

	SetAnchorPoint( 0.5f, 0.0f );

	//set position
	SetPositionType( pTag->GetPositionType() );
	SetPosition( pTag->GetPosition() );

	//set size
	SetSizeType( eScreenValue_Percentage );	
	CGUIImage* pBGImage = CGUIImageManager::Instance()->AllocateResource( "grass_upper" );
	GUI_ASSERT( pBGImage, "failed to get bg image");
	CGUISize aBGSize = pBGImage->GetSize();
	pBGImage->RefRelease();
	aBGSize.SetHeight( aBGSize.GetHeight() * 2 );
	CGUISize aMoleSize = GetCurrentAnimation()->GetSize();
	aMoleSize.SetWidth( aMoleSize.GetWidth() * GSystem->GetScreenWidth() / aBGSize.GetWidth()  );
	aMoleSize.SetHeight( aMoleSize.GetHeight() * GSystem->GetScreenHeight() / aBGSize.GetHeight() );
	SetPixelSize( aMoleSize );
	Refresh();

	//set position info
	m_aHidePosition = GetPosition();
	m_aPopPosition = m_aHidePosition;
	m_aPopPosition.y -= (aMoleSize.GetHeight() / GetParent()->GetPixelSize().GetHeight() );

	//set as
	m_pAs = CGUIAsManager::Instance()->AllocateResource<CGUIAsWidgetMoveTo>();
	m_pAs->SetReceiver( this );
	m_pAs->SetVelocity( (m_aHidePosition.y - m_aPopPosition.y) );
	SetAs( "working_as", m_pAs );
	m_pAs->RefRelease();
}
//------------------------------------------------------------------------------
bool CMyMole::IsTappable() const
{
	if( m_eMoleState == eMoleState_Popping || m_eMoleState == eMoleState_Popped )
	{
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
void CMyMole::UpdateMole(real fDeltaTime)
{
	switch( m_eMoleState )
	{
	case eMoleState_Hide:
		UpdateMole_Hide(fDeltaTime);
		break;
	case eMoleState_Popping:
		UpdateMole_Popping(fDeltaTime);
		break;
	case eMoleState_Popped:
		UpdateMole_Popped(fDeltaTime);
		break;
	case eMoleState_Shrinking:
		UpdateMole_Shrinking(fDeltaTime);
		break;
	}
}
//------------------------------------------------------------------------------
void CMyMole::UpdateMole_Hide(real fDeltaTime)
{
	if( rand() % 5 == 0 )
	{
		m_pAs->SetDestination( m_aPopPosition );
		m_pAs->Reset();
		PlayAs( m_pAs );
		m_eMoleState = eMoleState_Popping;
	}
}
//------------------------------------------------------------------------------
void CMyMole::UpdateMole_Popping(real fDeltaTime)
{
	if( !IsAsPlaying( m_pAs ) )
	{
		m_eMoleState = eMoleState_Popped;
		SetCurrentAnimation("laugh");
		GetCurrentAnimation()->Reset();
		m_fPoppedTime = 1.0f;
	}
}
//------------------------------------------------------------------------------
void CMyMole::ShrinkMole()
{
	m_pAs->SetDestination( m_aHidePosition );
	m_pAs->Reset();
	PlayAs( m_pAs );
	m_eMoleState = eMoleState_Shrinking;
}
//------------------------------------------------------------------------------
void CMyMole::UpdateMole_Popped(real fDeltaTime)
{
	if( GetCurrentAnimation()->IsPlaying() == false )
	{
		m_fPoppedTime -= fDeltaTime;
		if( m_fPoppedTime <= 0.0f )
		{
			ShrinkMole();
		}
	}
}
//------------------------------------------------------------------------------
void CMyMole::UpdateMole_Shrinking(real fDeltaTime)
{
	if( !IsAsPlaying( m_pAs ) )
	{
		m_eMoleState = eMoleState_Hide;
		SetCurrentAnimation("normal");
		GetCurrentAnimation()->Reset();
	}
}
//------------------------------------------------------------------------------
uint32 CMyMole::OnMouseLeftDown( CGUIEventMouse* pEvent )
{
	if( IsTappable() )
	{
		ShrinkMole();
		SetCurrentAnimation("thump");
		GetCurrentAnimation()->Reset();		
	}
	return CGUIWgtAnimation::OnMouseLeftDown( pEvent );
}
//------------------------------------------------------------------------------


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
	
	SetSizeType( eScreenValue_Percentage );
	SetSize( 1,1 );

	Refresh();

	CGUIWidget* pPanel = CGUIWidgetManager::Instance()->GetPage( "game_whackmole.xml", "game_whackmole" );
	pPanel->SetParent( this );
	pPanel->Refresh();

	//init mole
	for( uint32 i=0; i<3; ++i )
	{
		char szTagName[32];
		snprintf( szTagName, 32, "tag_mole_%d", i );
		CGUIWgtTag* pTag = CGUIWidgetManager::Instance()->GetWidgetWithTypeCheck<CGUIWgtTag>( szTagName, "game_whackmole" );
		GUI_ASSERT( pTag, "not found tag for mole" );

		char szMoleName[32];
		snprintf( szMoleName, 32, "mole_%d", i );
		CMyMole* pMole = CGUIWidgetManager::Instance()->CreateCustomWidget<CMyMole>( szMoleName, "" );
		pMole->InitMole( pTag );
		m_vecMole.push_back( pMole );
	}

	//init timer
	RegisterNativeCallbackFunc( "OnTimer", CMyCanvasLayer_WhackMoleGame::FunOnTimer );
	RegisterTimer( "UpdateTimer", 0.5f );

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
void CMyCanvasLayer_WhackMoleGame::FunOnTimer(CGUIEventTimer* pEvent )
{
	if( pEvent->GetTimerName() == "UpdateTimer")
	{
		for( uint32 i=0; i<uint32(CGUIFrameworkTest::ms_pFrameWork->GetGameLayer()->m_vecMole.size()); ++i )
		{
			CGUIFrameworkTest::ms_pFrameWork->GetGameLayer()->m_vecMole[i]->UpdateMole( pEvent->GetDuration() );
		}
	}
	else
	{
		GUI_FORCE_ASSERT("unknown timer name" );
	}
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_WhackMoleGame::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------



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
	GSystem->SetScreenOrientation( eScreenOrientation_LandscapeLeft );

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




