/** 
* @file game_turret.h
* @author ken
* @date 2011-04-14
*/


//============================================================================//
// include
//============================================================================// 
#include "game_turret.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 

CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}



//*****************************************************************************
//	CMyMonster
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyMonster );
//------------------------------------------------------------------------------
CMyMonster::CMyMonster( const CGUIString& rName, const CGUIString& rSceneName )
:CGUIWgtStaticImage( StaticGetType(), rName, rSceneName )
,m_bStrongMonster(false)
,m_fVelocity(0.0f)
,m_pTurret(NULL)
,m_uHp(0)
{
}
//------------------------------------------------------------------------------
void CMyMonster::InitMonster( CGUIWidget* pParent, bool bStrong, CGUIWgtStaticImage* pTurret )
{
	SetParent( pParent );
	m_pTurret = pTurret;

	m_bStrongMonster = bStrong;
	if( m_bStrongMonster )
	{
		m_fVelocity = 50.0f + rand() % 10;
		SetImage( "BGIMAGE", "target2" );
		m_uHp = 3;
	}
	else
	{
		m_fVelocity = 30.0f + rand() % 10;
		SetImage( "BGIMAGE", "target" );
		m_uHp = 1;
	}

	SetAnchorPoint( 0.5f, 0.5f );
	Create();
	Open();

	SetPixelPosition( GenerateTargetPosition() );

	CGUIAsCallFunc* pAsCallFunc = CGUIAsManager::Instance()->AllocateResource<CGUIAsCallFunc>();
	pAsCallFunc->SetReceiver( this );
	pAsCallFunc->SetFuncCallback( FunCallback_TargetMoveto );

	CGUIAsWidgetMoveTo* pAsMoveTo = CGUIAsManager::Instance()->AllocateResource<CGUIAsWidgetMoveTo>();
	pAsMoveTo->SetReceiver( this );
	pAsMoveTo->SetVelocity( m_fVelocity );
	pAsMoveTo->SetDestination( GenerateTargetPosition() );
	pAsMoveTo->AddSuccessor( pAsCallFunc );
	pAsCallFunc->RefRelease();

	SetAs( "As_MoveTo", pAsMoveTo );
	pAsMoveTo->RefRelease();

	PlayAs( pAsMoveTo );
}
//------------------------------------------------------------------------------
uint32 CMyMonster::GetHp() const
{
	return m_uHp;
}
//------------------------------------------------------------------------------
void CMyMonster::SetHp( uint32 uHp)
{
	m_uHp = uHp;
}
//------------------------------------------------------------------------------
void CMyMonster::FunCallback_TargetMoveto(CGUIAs* pAs)
{
	((CMyMonster*)pAs->GetReceiver())->ResetTargetPosition();
}
//------------------------------------------------------------------------------
void CMyMonster::ResetTargetPosition( )
{
	CGUIAsWidgetMoveTo* pAsMoveTo = static_cast<CGUIAsWidgetMoveTo*>(GetAs( "As_MoveTo" ));
	pAsMoveTo->Reset();
	pAsMoveTo->SetVelocity( m_fVelocity );
	pAsMoveTo->SetDestination( GenerateTargetPosition() );
	PlayAs( pAsMoveTo );
}
//------------------------------------------------------------------------------
CGUIVector2 CMyMonster::GenerateTargetPosition()
{
	CGUIVector2 aPos;
	aPos.x = real(rand( ) % uint32(GSystem->GetScreenWidth() - m_pTurret->GetPixelSize().GetWidth()) + m_pTurret->GetPixelSize().GetWidth());
	aPos.y = real(rand() % GSystem->GetScreenHeight());
	return aPos;
}
//------------------------------------------------------------------------------




//*****************************************************************************
//	CGUIAsShoot
//*****************************************************************************
//------------------------------------------------------------------------------
CGUIAsShoot::CGUIAsShoot( CMyCanvasLayer_TurretGame* pGameLayer )
:CGUICustomAs( "CGUIAsShoot" )
,m_pGameLayer( pGameLayer )
{
}
//------------------------------------------------------------------------------
void CGUIAsShoot::SetShootPos(const CGUIVector2& rShootPos)
{
	m_aShootPos = rShootPos;
}
//------------------------------------------------------------------------------
real CGUIAsShoot::Update( real fDeltaTime )
{
	if( !IsRetired() )
	{
		m_pGameLayer->Shoot( m_aShootPos );
		Retire( true );
	}

	return 0.0f;
}
//------------------------------------------------------------------------------


//*****************************************************************************
//	CMyProjectile
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyProjectile );
//------------------------------------------------------------------------------
CMyProjectile::CMyProjectile( const CGUIString& rName, const CGUIString& rSceneName )
:CGUIWgtStaticImage( StaticGetType(), rName, rSceneName )
,m_pTurret(NULL)
{
}
//------------------------------------------------------------------------------
void CMyProjectile::InitProjectile( CGUIWidget* pParent,CGUIWgtStaticImage* pTurret, const CGUIVector2& rShootPos )
{
	m_pTurret = pTurret;
	CGUIVector2 aDelta = rShootPos - m_pTurret->GetPixelPosition();

	//calculate begin position
	CGUIVector2 aBeginPos(m_pTurret->GetPixelSize().GetWidth()/2.0f, 0.0f);
	CGUIMatrix3 aRotMat;
	aRotMat.FromEulerAnglesXYZ( 0, 0, CGUIMath::DegreesToRadians(m_pTurret->GetRotation().z ));
	CGUIQuaternion quat(aRotMat);
	CGUIVector3 aTmpPos( aBeginPos.x, aBeginPos.y, 0.0f );
	aTmpPos = quat * aTmpPos;
	aBeginPos.x = aTmpPos.x;
	aBeginPos.y = aTmpPos.y;
	aBeginPos += m_pTurret->GetPixelPosition();

	//calculate final position
	CGUIVector2 aFinalPos;
	aFinalPos.x = real(GSystem->GetScreenWidth());
	aFinalPos.y = aDelta.y / aDelta.x * aFinalPos.x;
	if( abs( aFinalPos.y ) > real(GSystem->GetScreenHeight() / 2.0f) )
	{
		real fRatio = real(GSystem->GetScreenHeight() / 2.0f) / abs( aFinalPos.y );
		aFinalPos.x *= fRatio;
		aFinalPos.y *= fRatio;
	}
	aFinalPos.y += real(GSystem->GetScreenHeight() / 2.0f);

	//create projectile
	SetParent( pParent );
	SetImage( "BGIMAGE", "projectile" );
	SetPixelPosition( aBeginPos );
	SetAnchorPoint( 0.5f, 0.5f );
	Create();
	Open();

	//set projectile as
	CGUIAsCallFunc* pAsCallFunc = CGUIAsManager::Instance()->AllocateResource<CGUIAsCallFunc>();
	pAsCallFunc->SetReceiver( this );
	pAsCallFunc->SetFuncCallback( FunCallback_ProjectileFinish );

	CGUIAsWidgetMoveTo* pAsMoveTo = CGUIAsManager::Instance()->AllocateResource<CGUIAsWidgetMoveTo>();
	pAsMoveTo->SetReceiver( this );
	pAsMoveTo->SetVelocity( 480.f );
	pAsMoveTo->SetDestination( aFinalPos );
	pAsMoveTo->AddSuccessor( pAsCallFunc );
	pAsCallFunc->RefRelease();

	SetAs( "As_MoveTo", pAsMoveTo );
	pAsMoveTo->RefRelease();
	PlayAs( pAsMoveTo );

	//play sound
	CGUIInterfaceManager::Instance()->GetInterfaceSound()->PlayEffect( "se_selection.ogg" );
}
//------------------------------------------------------------------------------
void CMyProjectile::FunCallback_ProjectileFinish(CGUIAs* pAs)
{
	CGUIFrameworkTest::ms_pFrameWork->GetTurretGameLayer()->DestroyProjectile( (CMyProjectile*)pAs->GetReceiver() );
}
//------------------------------------------------------------------------------



//*****************************************************************************
//	CMyCanvasLayer_TurretGame
//*****************************************************************************

//------------------------------------------------------------------------------
CMyCanvasLayer_TurretGame::CMyCanvasLayer_TurretGame( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
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
	InitTurret();

	//init target
	for( uint32 i=0; i<5; ++i )
	{
		char szTargetName[32];
		snprintf( szTargetName, 32, "monster_%d", i );
		CMyMonster* pMonster = CGUIWidgetManager::Instance()->CreateCustomWidget<CMyMonster>( szTargetName, "" );
		pMonster->InitMonster( m_pTurret->GetParent(), (i%2)==0, m_pTurret );
		m_vecTarget.push_back( pMonster );
	}

	//init score
	m_pScore = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtStaticText>( "score", "");
	m_pScore->SetParent( GSystem->GetUICanvas() );
	m_pScore->SetSize( 50, 25 );
	m_pScore->SetPosition( 0,0 );
	m_pScore->SetTextInfo(CGUIStringRenderInfo(0, 16, CGUIColor(1,1,1,1)));
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
void CMyCanvasLayer_TurretGame::DestroyProjectile( CMyProjectile* pProjectile )
{
	m_vecProjectile.erase( std::find( m_vecProjectile.begin(), m_vecProjectile.end(), pProjectile ));
	pProjectile->Close();
	pProjectile->SetParent( NULL );
	CGUIWidgetManager::Instance()->DelayedDestroyWidget( pProjectile );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::Btn_OnClick_Panel( CGUIEventMouse* pEvent )
{
	CGUIFrameworkTest::ms_pFrameWork->GetTurretGameLayer()->BeginShoot( pEvent->GetLocalPosition() );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::OnUpdate(real fDeltaTime)
{
	CGUICanvasLayer::OnUpdate(fDeltaTime );

	std::vector<CMyProjectile *> projectilesToDelete;
	for ( std::vector<CMyProjectile *>::iterator itorProjectile = m_vecProjectile.begin();
		itorProjectile != m_vecProjectile.end();
		++itorProjectile )
	{
		CMyProjectile* pProjectile = *itorProjectile;
		CGUIRect aProjectileRect( 
			pProjectile->GetPixelPosition().x - pProjectile->GetPixelSize().GetWidth()/2,
			pProjectile->GetPixelPosition().y - pProjectile->GetPixelSize().GetHeight()/2,
			pProjectile->GetPixelPosition().x + pProjectile->GetPixelSize().GetWidth()/2,
			pProjectile->GetPixelPosition().y + pProjectile->GetPixelSize().GetHeight()/2);

		std::vector<CMyMonster *> targetsToDelete;
		for ( std::vector<CMyMonster *>::iterator itorTarget = m_vecTarget.begin();
			itorTarget != m_vecTarget.end();
			++itorTarget )
		{
			CMyMonster* pTarget = *itorTarget;
			CGUIRect aTargetRect( 
				pTarget->GetPixelPosition().x - pTarget->GetPixelSize().GetWidth()/2,
				pTarget->GetPixelPosition().y - pTarget->GetPixelSize().GetHeight()/2,
				pTarget->GetPixelPosition().x + pTarget->GetPixelSize().GetWidth()/2,
				pTarget->GetPixelPosition().y + pTarget->GetPixelSize().GetHeight()/2);

			if( aTargetRect.IsIntersection(aProjectileRect) )
			{
				targetsToDelete.push_back( pTarget );
				break;
			}
		}

		for ( std::vector<CMyMonster *>::iterator itor = targetsToDelete.begin();
			itor != targetsToDelete.end();
			++itor )
		{
			CMyMonster* pMonster = *itor;
			pMonster->SetHp( pMonster->GetHp()-1 );
			if( pMonster->GetHp() == 0 )
			{
				DestroyTarget( *itor );
				IncreaseScroe();
			}
		}

		if (targetsToDelete.size() > 0) 
		{
			projectilesToDelete.push_back(pProjectile);
		}
		targetsToDelete.clear();
	}

	for ( std::vector<CMyProjectile *>::iterator itor = projectilesToDelete.begin();
		itor != projectilesToDelete.end();
		++itor )
	{
		DestroyProjectile( *itor );
	}
	projectilesToDelete.clear();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::DestroySelf( )
{
	delete this;
}

//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::InitTurret()
{
	CGUIAsShoot* pAsShoot = new CGUIAsShoot( this );
	pAsShoot->SetReceiver( m_pTurret );
	pAsShoot->RefRetain();

	CGUIAsWidgetRotation* pAsTurnTo = CGUIAsManager::Instance()->AllocateResource<CGUIAsWidgetRotation>();
	pAsTurnTo->SetReceiver( m_pTurret );
	CGUIVector3 aCurRotation = m_pTurret->GetRotation();
	CGUIVector3 aDestRotation = aCurRotation;
	real rotateSpeed = 360.0f; // Would take 0.5 seconds to rotate 0.5 radians, or half a circle
	real rotateDuration = fabs((aDestRotation.z - aCurRotation.z) / rotateSpeed);   
	pAsTurnTo->SetInterpolationValue( aCurRotation, aDestRotation, rotateDuration );
	pAsTurnTo->AddSuccessor( pAsShoot );
	pAsShoot->RefRelease();

	m_pTurret->SetAs( "As_TurnTo", pAsTurnTo );
	pAsTurnTo->RefRelease();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::BeginShoot( const CGUIVector2& rShootPos )
{
	if( rShootPos.x <= m_pTurret->GetPixelPosition().x + m_pTurret->GetPixelSize().GetWidth() )
	{
		return;
	}

	CGUIVector2 aDelta = rShootPos - m_pTurret->GetPixelPosition();

	//set rotation of turret
	real angleRadians = atanf( aDelta.y / aDelta.x );
	real angleDegrees = CGUIMath::RadiansToDegrees(angleRadians);

	//set turret as
	CGUIAsWidgetRotation* pAsTurnTo = (CGUIAsWidgetRotation*)m_pTurret->GetAs("As_TurnTo");
	pAsTurnTo->Reset();
	CGUIVector3 aCurRotation = m_pTurret->GetRotation();
	CGUIVector3 aDestRotation = aCurRotation;
	aDestRotation.z = angleDegrees;
	real rotateSpeed = 360.0f; // Would take 0.5 seconds to rotate 0.5 radians, or half a circle
	real rotateDuration = fabs((aDestRotation.z - aCurRotation.z) / rotateSpeed);   
	pAsTurnTo->SetInterpolationValue( aCurRotation, aDestRotation, rotateDuration );
	static_cast<CGUIAsShoot*>(pAsTurnTo->GetSuccessor( 0 ))->SetShootPos(rShootPos);

	m_pTurret->PlayAs( pAsTurnTo );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::Shoot( const CGUIVector2& rShootPos )
{
	//create projectile
	static uint32 uProjectileIdx = 0;
	char szProjectileName[32];
	snprintf( szProjectileName, 32, "projectile_%d", uProjectileIdx++ );
	CMyProjectile* pProjectile = CGUIWidgetManager::Instance()->CreateCustomWidget<CMyProjectile>( szProjectileName, "");
	pProjectile->InitProjectile( m_pTurret->GetParent(), m_pTurret, rShootPos );
	m_vecProjectile.push_back(pProjectile);
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::IncreaseScroe( )
{
	//update score
	CGUIString strScroe = m_pScore->GetTextContentUTF8();
	uint32 uScore;
	StringToValue( strScroe, uScore );
	++uScore;
	ValueToString( uScore, strScroe );
	m_pScore->SetTextContentUTF8( strScroe );
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_TurretGame::DestroyTarget( CGUIWidget* pTarget )
{
	//destroy target
	m_vecTarget.erase( std::find( m_vecTarget.begin(), m_vecTarget.end(), pTarget ));
	pTarget->Close();
	pTarget->SetParent( NULL );
	CGUIWidgetManager::Instance()->DelayedDestroyWidget( pTarget );
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
,m_pTurretGameLayer(NULL)
{
	ms_pFrameWork = this;
}
//------------------------------------------------------------------------------
CMyCanvasLayer_TurretGame* CGUIFrameworkTest::GetTurretGameLayer()
{
	return m_pTurretGameLayer;
}
//------------------------------------------------------------------------------
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




