#include <libguiex_framework/guiframework.h>
using namespace guiex;

class CMyCanvasLayer_TurretGame : public CGUICanvasLayer
{
public:
	CMyCanvasLayer_TurretGame( const char* szLayerName );
	~CMyCanvasLayer_TurretGame(  );

	virtual void DestroySelf( );

	void ResetTargetPosition();

	void Shoot( const CGUIVector2& rShootPos );
	void FinishProjectile( CGUIWidget* pProjectile );

protected:
	virtual void RenderSelf(IGUIInterfaceRender* pRender);

	//target related functions
	void InitTarget();
	CGUIVector2 GenerateTargetPosition();


protected:
	CGUIWgtStaticImage* m_pTarget;
	CGUIWgtStaticImage* m_pTurret;
	
	CGUIWgtStaticText* m_pScore;
};

class CGUIFrameworkTest : public CGUIFramework
{
public:
	CGUIFrameworkTest(  )
		:CGUIFramework(  )
		,m_pTurretGameLayer(NULL)
	{
		ms_pFrameWork = this;
	}

	CMyCanvasLayer_TurretGame* GetTurretGameLayer()
	{
		return m_pTurretGameLayer;
	}

	static CGUIFrameworkTest* ms_pFrameWork;

protected:
	virtual int32 InitializeGame( );

protected:
	CMyCanvasLayer_TurretGame* m_pTurretGameLayer;
};
CGUIFrameworkTest* CGUIFrameworkTest::ms_pFrameWork = NULL;

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}

int32 CGUIFrameworkTest::InitializeGame()
{
	CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");
	
	CGUISceneManager::Instance()->LoadResources( "common" );	
	CGUISceneManager::Instance()->LoadWidgets( "common" );

	CGUISceneManager::Instance()->LoadResources( "game_turret" );	
	CGUISceneManager::Instance()->LoadWidgets( "game_turret" );

	//add utility widget
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);		

	//create layer
	m_pTurretGameLayer = new CMyCanvasLayer_TurretGame( "turret game layer" );
	m_pTurretGameLayer->Initialize();
	CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pTurretGameLayer );
	return 0;
}




//------------------------------------------------------------------------------
//CMyCanvasLayer_TurretGame
//------------------------------------------------------------------------------
void Btn_OnClick_Panel( CGUIEventMouse* pEvent )
{
	CGUIFrameworkTest::ms_pFrameWork->GetTurretGameLayer()->Shoot( pEvent->GetLocalPosition() );
}
//------------------------------------------------------------------------------
CMyCanvasLayer_TurretGame::CMyCanvasLayer_TurretGame( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
,m_pTarget( NULL )
,m_pTurret( NULL )
,m_pScore(NULL)
{
	//set attribute
	SetHitable( true );
	SetGenerateClickEvent( true );
	SetMouseConsumed(true);

	CGUIWidget* pPanel = CGUIWidgetManager::Instance()->GetPage( "game_turret.xml", "game_turret" );
	pPanel->SetParent( this );	
	pPanel->RegisterNativeCallbackFunc( "OnMouseLeftClick", Btn_OnClick_Panel );

	//init turret
	m_pTurret = CGUIWidgetManager::Instance()->GetWidgetWithTypeCheck<CGUIWgtStaticImage>( "turret", "game_turret" );
	m_pTurret->SetPixelPosition( m_pTurret->GetPixelSize().GetWidth() / 2.0f, GSystem->GetScreenHeight() / 2.0f );

	//init target
	m_pTarget = CGUIWidgetManager::Instance()->GetWidgetWithTypeCheck<CGUIWgtStaticImage>( "target", "game_turret" );
	InitTarget();

	//init score
	m_pScore = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "score", "");
	m_pScore->SetParent( GSystem->GetUICanvas() );
	m_pScore->SetSize( 50, 25 );
	m_pScore->SetPosition( 0,0 );
	m_pScore->SetTextInfo(CGUIStringInfo(0, 16, CGUIColor(1,1,1,1)));
	m_pScore->SetTextAlignmentHorz( eTextAlignment_Horz_Left );
	m_pScore->SetTextContent(L"0");
	m_pScore->Create();
	m_pScore->Open();

}

//------------------------------------------------------------------------------
CMyCanvasLayer_TurretGame::~CMyCanvasLayer_TurretGame(  )
{
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::RenderSelf(IGUIInterfaceRender* pRender)
{
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------
void FunRetiredCallback_TargetMoveto(CGUIAs* pAs)
{
	CGUIFrameworkTest::ms_pFrameWork->GetTurretGameLayer()->ResetTargetPosition();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::InitTarget()
{
	m_pTarget->SetPixelPosition( GenerateTargetPosition() );

	CGUIAsCallFunc* pAsCallFunc = CGUIAsManager::Instance()->AllocateResource<CGUIAsCallFunc>();
	pAsCallFunc->SetReceiver( m_pTarget );
	pAsCallFunc->SetFuncCallback( FunRetiredCallback_TargetMoveto );

	CGUIAsMoveTo* pAsMoveTo = CGUIAsManager::Instance()->AllocateResource<CGUIAsMoveTo>();
	pAsMoveTo->SetReceiver( m_pTarget );
	pAsMoveTo->SetVelocity( 30.0f + rand() % 10 );
	pAsMoveTo->SetDestination( GenerateTargetPosition() );
	pAsMoveTo->AddSuccessor( pAsCallFunc );
	pAsCallFunc->RefRelease();

	m_pTarget->SetAs( "As_MoveTo", pAsMoveTo );
	pAsMoveTo->RefRelease();

	m_pTarget->PlayAs( pAsMoveTo );
}
//------------------------------------------------------------------------------
CGUIVector2 CMyCanvasLayer_TurretGame::GenerateTargetPosition()
{
	CGUIVector2 aPos;
	aPos.x = real(rand( ) % uint32(GSystem->GetScreenWidth() - m_pTurret->GetPixelSize().GetWidth()) + m_pTurret->GetPixelSize().GetWidth());
	aPos.y = real(rand() % GSystem->GetScreenHeight());
	return aPos;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::ResetTargetPosition()
{
	CGUIAsMoveTo* pAsMoveTo = (CGUIAsMoveTo*)m_pTarget->GetAs( "As_MoveTo" );
	pAsMoveTo->Reset();
	pAsMoveTo->SetVelocity( 30.0f + rand() % 10 );
	pAsMoveTo->SetDestination( GenerateTargetPosition() );
	m_pTarget->PlayAs( pAsMoveTo );
}
//------------------------------------------------------------------------------
void FunRetiredCallback_ProjectileFinish(CGUIAs* pAs)
{
	CGUIFrameworkTest::ms_pFrameWork->GetTurretGameLayer()->FinishProjectile( pAs->GetReceiver() );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::Shoot( const CGUIVector2& rShootPos )
{
	if( rShootPos.x <= m_pTurret->GetPixelPosition().x + m_pTurret->GetPixelSize().GetWidth() )
	{
		return;
	}

	//create projectile
	static uint32 uProjectileIdx = 0;
	char szProjectileName[32];
	snprintf( szProjectileName, 32, "projectile_%d", uProjectileIdx++ );
	CGUIWgtStaticImage* pProjectile = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticImage>( szProjectileName, "");
	pProjectile->SetParent( this );
	pProjectile->SetImage( "BGIMAGE", "projectile" );
	pProjectile->SetPixelPosition( m_pTurret->GetPixelPosition().x + m_pTurret->GetPixelSize().GetWidth()/2.0f, m_pTurret->GetPixelPosition().y );
	pProjectile->SetAnchorPoint( 0.5f, 0.5f );
	pProjectile->Create();
	pProjectile->Open();

	//calculate final position
	CGUIVector2 aDelta = rShootPos - pProjectile->GetPixelPosition();
	CGUIVector2 aFinalPos;
	aFinalPos.x = real(GSystem->GetScreenWidth());
	aFinalPos.y = aDelta.y / aDelta.x * aFinalPos.x;
	if( abs( aFinalPos.y ) > real(GSystem->GetScreenHeight() / 2.0f) )
	{
		real fRatio = real(GSystem->GetScreenHeight() / 2.0f) / aFinalPos.y;
		aFinalPos.x *= fRatio;
		aFinalPos.y *= fRatio;
	}

	//set as
	CGUIAsCallFunc* pAsCallFunc = CGUIAsManager::Instance()->AllocateResource<CGUIAsCallFunc>();
	pAsCallFunc->SetReceiver( pProjectile );
	pAsCallFunc->SetFuncCallback( FunRetiredCallback_ProjectileFinish );

	CGUIAsMoveTo* pAsMoveTo = CGUIAsManager::Instance()->AllocateResource<CGUIAsMoveTo>();
	pAsMoveTo->SetReceiver( pProjectile );
	pAsMoveTo->SetVelocity( 180.f );
	pAsMoveTo->SetDestination( aFinalPos );
	pAsMoveTo->AddSuccessor( pAsCallFunc );
	pAsCallFunc->RefRelease();

	pProjectile->SetAs( "As_MoveTo", pAsMoveTo );
	pAsMoveTo->RefRelease();
	pProjectile->PlayAs( pAsMoveTo );

}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::FinishProjectile( CGUIWidget* pProjectile )
{
	pProjectile->Close();
	pProjectile->SetParent( NULL );
	CGUIWidgetManager::Instance()->DelayedDestroyWidget( pProjectile );
}
//------------------------------------------------------------------------------
