/** 
* @file game_breakout.cpp
* @author ken
* @date 2011-04-18
*/


//============================================================================//
// include
//============================================================================// 
#include "game_breakout.h"
#include <algorithm>

//============================================================================//
// function
//============================================================================// 

guiex::CGUIFrameworkBase* CreateFramework( )
{
	return new CGUIFrameworkTest( );
}


//*****************************************************************************
//	CMyBodyBase
//*****************************************************************************
//------------------------------------------------------------------------------
CMyBodyBase::CMyBodyBase( const guiex::CGUIString& rType, const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName, EBodyType eType )
:guiex::CGUIWgtStaticImage( rType, rName, rSceneName )
,m_pBody(NULL)
,m_pFixture(NULL)
,m_eType( eType )
{
}
//------------------------------------------------------------------------------
void CMyBodyBase::OnDestroy()
{
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
	if( m_pBody )
	{
		pPhysics->GetWorld()->DestroyBody( m_pBody );
	}
	guiex::CGUIWgtStaticImage::OnDestroy( );
}
//------------------------------------------------------------------------------
b2Body* CMyBodyBase::GetBody()
{
	return m_pBody;
}
//------------------------------------------------------------------------------
b2Fixture* CMyBodyBase::GetFixture()
{
	return m_pFixture;
}
//------------------------------------------------------------------------------
EBodyType CMyBodyBase::GetBodyType() const
{
	return m_eType;
}
//------------------------------------------------------------------------------
void CMyBodyBase::OnUpdate( guiex::real fDeltaTime )
{
	guiex::CGUIWgtStaticImage::OnUpdate(fDeltaTime );

	const b2Vec2& rPos = m_pBody->GetPosition();
	guiex::CGUIVector2 aPos( guiex::IGUIPhysics_box2d::Meter2Pixel(rPos.x), guiex::IGUIPhysics_box2d::Meter2Pixel(rPos.y) );
	guiex::real fRot = m_pBody->GetAngle() / b2_pi * 180.f;

	SetPixelPosition( aPos );
	SetRotation( 0, 0, fRot );

	Refresh();
}
//------------------------------------------------------------------------------

//*****************************************************************************
//	CMyBall
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyBall );
//------------------------------------------------------------------------------
CMyBall::CMyBall( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName )
:CMyBodyBase( StaticGetType(), rName, rSceneName, eBodyType_Ball )
{

}
//------------------------------------------------------------------------------
void CMyBall::InitBall( guiex::CGUIWidget* pParent )
{
	SetParent( pParent );
	SetImage( "bg", "Ball" );
	SetAnchorPoint( 0.5f, 0.5f );
	SetPixelPosition( 100, guiex::GSystem->GetScreenHeight() - 100.0f );
	Refresh();

	// Create ball body 
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
	guiex::real fScale = pPhysics->GetMeterPixelScale();

	b2BodyDef ballBodyDef;
	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(GetPixelPosition().x/fScale, GetPixelPosition().y/fScale);
	ballBodyDef.userData = this;
	m_pBody = pPhysics->GetWorld()->CreateBody(&ballBodyDef);

	// Create circle shape
	b2CircleShape circle;
	circle.m_radius = GetPixelSize().GetWidth()/2/fScale;

	// Create shape definition and add to body
	b2FixtureDef ballShapeDef;
	ballShapeDef.shape = &circle;
	ballShapeDef.density = 1.0f;
	ballShapeDef.friction = 0.f;
	ballShapeDef.restitution = 1.0f;
	m_pFixture = m_pBody->CreateFixture(&ballShapeDef);

	//add force
	b2Vec2 force = b2Vec2(10, -10);
	m_pBody->ApplyLinearImpulse(force, ballBodyDef.position);
}
//------------------------------------------------------------------------------
void CMyBall::OnUpdate(guiex::real fDeltaTime)
{
	CMyBodyBase::OnUpdate(fDeltaTime );

	//adjust velocity
	static guiex::real maxSpeed = 10;

	b2Vec2 velocity = m_pBody->GetLinearVelocity();
	float32 speed = velocity.Length();
	if (speed > maxSpeed) 
	{
		m_pBody->SetLinearDamping(0.5);
	}
	else if (speed < maxSpeed) 
	{
		if( speed == 0.0f )
		{
			velocity = b2Vec2(10, 10);
		}
		else
		{
			velocity.Normalize();
			velocity *= 10.0f;
		}

		m_pBody->SetLinearVelocity(velocity);
		m_pBody->SetLinearDamping(0.0);
	}
}
//------------------------------------------------------------------------------



//*****************************************************************************
//	CMyPaddle
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyPaddle );
//------------------------------------------------------------------------------
CMyPaddle::CMyPaddle( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName )
:CMyBodyBase( StaticGetType(), rName, rSceneName, eBodyType_Paddle )
,m_mouseJoint(NULL)
,m_pPrismaticJoint(NULL)
{

}
//------------------------------------------------------------------------------
void CMyPaddle::InitPaddle( guiex::CGUIWidget* pParent )
{
	SetParent( pParent );
	SetImage( "bg", "Paddle" );
	SetHitable( true );
	SetMovable( true );
	SetAnchorPoint( 0.5f, 0.5f );
	SetPixelPosition( guiex::GSystem->GetScreenWidth() / 2.0f, guiex::GSystem->GetScreenHeight() - 50.0f );
	Refresh();

	// Create paddle body 
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
	guiex::real fScale = pPhysics->GetMeterPixelScale();
	guiex::real winWidth = guiex::real(guiex::GSystem->GetScreenWidth());
	guiex::real winHeight = guiex::real(guiex::GSystem->GetScreenHeight());

	b2BodyDef paddleBodyDef;
	paddleBodyDef.type = b2_dynamicBody;
	paddleBodyDef.position.Set(GetPixelPosition().x/fScale, GetPixelPosition().y/fScale);
	paddleBodyDef.userData = this;
	m_pBody = pPhysics->GetWorld()->CreateBody(&paddleBodyDef);

	// Create paddle shape
	b2PolygonShape paddleShape;
	paddleShape.SetAsBox(GetPixelSize().GetWidth()/fScale/2, GetPixelSize().GetHeight()/fScale/2);

	// Create shape definition and add to body
	b2FixtureDef paddleShapeDef;
	paddleShapeDef.shape = &paddleShape;
	paddleShapeDef.density = 10.0f;
	paddleShapeDef.friction = 0.4f;
	paddleShapeDef.restitution = 0.1f;
	m_pFixture = m_pBody->CreateFixture(&paddleShapeDef);

	// Restrict paddle along the x axis
	b2PrismaticJointDef jointDef;
	b2Vec2 worldAxis(1.0f, 0.0f);
	jointDef.collideConnected = true;
	jointDef.Initialize(m_pBody, CGUIFrameworkTest::ms_pFrameWork->GetGameLayer()->GetGroundBody(), m_pBody->GetWorldCenter(), worldAxis);
	m_pPrismaticJoint = pPhysics->GetWorld()->CreateJoint(&jointDef);
}
//------------------------------------------------------------------------------
uint32 CMyPaddle::OnDragBegin( guiex::CGUIEventDrag* pEvent )
{
	if (m_mouseJoint != NULL) 
	{
		return 0;
	}

	b2Vec2 locationWorld = b2Vec2( guiex::IGUIPhysics_box2d::Pixel2Meter(pEvent->GetMouseGlobalPos().x), guiex::IGUIPhysics_box2d::Pixel2Meter(pEvent->GetMouseGlobalPos().y));

	if (m_pFixture->TestPoint(locationWorld)) 
	{
		guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();

		b2MouseJointDef md;
		md.bodyA = CGUIFrameworkTest::ms_pFrameWork->GetGameLayer()->GetGroundBody();
		md.bodyB = m_pBody;
		md.target = locationWorld;
		md.collideConnected = true;
		md.maxForce = 1000.0f * m_pBody->GetMass();

		m_mouseJoint = (b2MouseJoint *)pPhysics->GetWorld()->CreateJoint(&md);
		m_pBody->SetAwake(true);
	}

	return CMyBodyBase::OnDragBegin(pEvent);
}
//------------------------------------------------------------------------------
uint32 CMyPaddle::OnDragProcess( guiex::CGUIEventDrag* pEvent )
{
	if (m_mouseJoint == NULL) 
	{
		return 0;
	}

	b2Vec2 locationWorld = b2Vec2( guiex::IGUIPhysics_box2d::Pixel2Meter(pEvent->GetMouseGlobalPos().x), guiex::IGUIPhysics_box2d::Pixel2Meter(pEvent->GetMouseGlobalPos().y));

	m_mouseJoint->SetTarget(locationWorld);

	pEvent->Expire( true );

	return CMyBodyBase::OnDragProcess(pEvent);
}
//------------------------------------------------------------------------------
uint32 CMyPaddle::OnDragEnd( guiex::CGUIEventDrag* pEvent )
{
	if (m_mouseJoint) 
	{
		guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
		pPhysics->GetWorld()->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}

	return CMyBodyBase::OnDragEnd(pEvent);
}
//------------------------------------------------------------------------------




//*****************************************************************************
//	CMyBlock
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyBlock );
//------------------------------------------------------------------------------
CMyBlock::CMyBlock( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName )
:CMyBodyBase( StaticGetType(), rName, rSceneName, eBodyType_Block )
{

}
//------------------------------------------------------------------------------
void CMyBlock::InitBlock( guiex::CGUIWidget* pParent, const guiex::CGUIVector2& rPos )
{
	SetParent( pParent );
	SetImage( "bg", "Block" );
	SetAnchorPoint( 0.5f, 0.5f );
	SetPixelPosition( rPos );
	Refresh();

	// Create ball body 
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
	guiex::real fScale = pPhysics->GetMeterPixelScale();

	b2BodyDef blockBodyDef;
	blockBodyDef.type = b2_dynamicBody;
	blockBodyDef.position.Set(GetPixelPosition().x/fScale, GetPixelPosition().y/fScale);
	blockBodyDef.userData = this;
	m_pBody = pPhysics->GetWorld()->CreateBody(&blockBodyDef);

	// Create block shape
	b2PolygonShape blockShape;
	blockShape.SetAsBox(GetPixelSize().GetWidth()/fScale/2, GetPixelSize().GetHeight()/fScale/2);

	// Create shape definition and add to body
	b2FixtureDef blockShapeDef;
	blockShapeDef.shape = &blockShape;
	blockShapeDef.density = 10.0;
	blockShapeDef.friction = 0.0;
	blockShapeDef.restitution = 0.1f;
	m_pFixture = m_pBody->CreateFixture(&blockShapeDef);
}
//------------------------------------------------------------------------------



//*****************************************************************************
//	MyContactListener
//*****************************************************************************
//------------------------------------------------------------------------------
MyContactListener::MyContactListener()
{
}
//------------------------------------------------------------------------------
MyContactListener::~MyContactListener() 
{
}
//------------------------------------------------------------------------------
void MyContactListener::BeginContact(b2Contact* contact)
{
	// We need to copy out the data because the b2Contact passed in
	// is reused.
	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	_contacts.push_back(myContact);
}
//------------------------------------------------------------------------------
void MyContactListener::EndContact(b2Contact* contact) 
{
	MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
	std::vector<MyContact>::iterator pos;
	pos = std::find(_contacts.begin(), _contacts.end(), myContact);
	if (pos != _contacts.end()) 
	{
		_contacts.erase(pos);
	}
}
//------------------------------------------------------------------------------
void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) 
{
}
//------------------------------------------------------------------------------
void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
{
}
//------------------------------------------------------------------------------


//*****************************************************************************
//	CMyCanvasLayer_GameLayer
//*****************************************************************************

//------------------------------------------------------------------------------
CMyCanvasLayer_GameLayer::CMyCanvasLayer_GameLayer( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
,m_groundBody(NULL)
,m_bottomFixture(NULL)
,m_pPhysics(NULL)
,m_contactListener(NULL)
,m_pBall(NULL)
,m_pBackground(NULL)
{
	//set attribute
	SetHitable( true );
	SetGenerateClickEvent( true );
	SetMouseConsumed(true);

	//init physics
	m_pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
	m_pPhysics->SetMeterPixelScale( 32.0f );

	//set gravity
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	m_pPhysics->GetWorld()->SetGravity( gravity );

	// Create edges around the entire screen
	guiex::real winWidth = guiex::real(guiex::GSystem->GetScreenWidth());
	guiex::real winHeight = guiex::real(guiex::GSystem->GetScreenHeight());
	guiex::real fScale = m_pPhysics->GetMeterPixelScale();
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	m_groundBody = m_pPhysics->GetWorld()->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	b2FixtureDef groundBoxDef;
	groundBoxDef.shape = &groundBox;
	groundBox.SetAsEdge(b2Vec2(0,0), b2Vec2(winWidth/fScale, 0));
	m_groundBody->CreateFixture(&groundBoxDef);
	groundBox.SetAsEdge(b2Vec2(0,0), b2Vec2(0, winHeight/fScale));
	m_groundBody->CreateFixture(&groundBoxDef);
	groundBox.SetAsEdge(b2Vec2(0, winHeight/fScale), b2Vec2(winWidth/fScale, winHeight/fScale));
	m_bottomFixture = m_groundBody->CreateFixture(&groundBoxDef);
	groundBox.SetAsEdge(b2Vec2(winWidth/fScale, winHeight/fScale), b2Vec2(winWidth/fScale, 0));
	m_groundBody->CreateFixture(&groundBoxDef);

	m_contactListener = new MyContactListener();
	m_pPhysics->GetWorld()->SetContactListener(m_contactListener);
}
//------------------------------------------------------------------------------
CMyCanvasLayer_GameLayer::~CMyCanvasLayer_GameLayer(  )
{
	if( m_contactListener )
	{
		delete m_contactListener;
		m_contactListener = NULL;
	}

	if( m_groundBody )
	{
		m_pPhysics->GetWorld()->DestroyBody( m_groundBody );
		m_groundBody = NULL;
		m_bottomFixture = NULL;
	}

}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::Initialize( )
{
	guiex::CGUICanvasLayer::Initialize();

	//load background widget
	m_pBackground = guiex::CGUIWidgetManager::Instance()->LoadPage( "game_breakout.xml", "game_breakout" );
	m_pBackground->SetParent( this );

	// Create sprite and add it to the layer
	m_pBall = guiex::CGUIWidgetManager::Instance()->CreateCustomWidget<CMyBall>( "ball", "" );
	m_pBall->InitBall( m_pBackground );

	// Create paddle and add it to the layer
	CMyPaddle* pPaddle = guiex::CGUIWidgetManager::Instance()->CreateCustomWidget<CMyPaddle>( "paddle", "" );
	pPaddle->InitPaddle( m_pBackground );

	// Create block and add it to the layer
	for(int i = 0; i < 4; i++) 
	{
		static int padding=20;
		static int nBlockWidth=90;
		int leftPadding = (guiex::GSystem->GetScreenWidth() - nBlockWidth*4 - padding*3)/2;
		char szName[32];
		snprintf( szName, 32, "block_%d", i );
		guiex::real xOffset = guiex::real(leftPadding+nBlockWidth/2+((nBlockWidth+padding)*i));
		guiex::CGUIVector2 aPos( xOffset, 100.0f );
		CMyBlock* pBlock = guiex::CGUIWidgetManager::Instance()->CreateCustomWidget<CMyBlock>( szName, "" );
		pBlock->InitBlock( m_pBackground, aPos );

		m_arrayBlocks.insert( pBlock );
	}

	m_pBackground->Open();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::Finalize( )
{
	if( m_pBackground )
	{
		guiex::CGUIWidgetManager::Instance()->DestroyWidget( m_pBackground );
		m_pBackground = NULL;
	}
	guiex::CGUICanvasLayer::Finalize();
}
//------------------------------------------------------------------------------
b2Body* CMyCanvasLayer_GameLayer::GetGroundBody()
{
	return m_groundBody;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::OnUpdate(guiex::real fDeltaTime)
{
	CGUICanvasLayer::OnUpdate(fDeltaTime );

	std::vector<b2Body *> toDestroy;
	std::vector<MyContact>::iterator pos;
	for(pos = m_contactListener->_contacts.begin(); 
		pos != m_contactListener->_contacts.end(); ++pos)
	{
		MyContact contact = *pos;

		if ((contact.fixtureA == m_bottomFixture && contact.fixtureB == m_pBall->GetFixture()) ||
			(contact.fixtureA == m_pBall->GetFixture() && contact.fixtureB == m_bottomFixture)) 
		{
			//lose
			if( CGUIFrameworkTest::ms_pFrameWork->GetMenuLayer() == NULL )
			{
				CGUIFrameworkTest::ms_pFrameWork->BeginOpenMenuLayer("scene_lose.xml");
				SetDisable( true );
				m_pPhysics->PauseSimulate();
				return;
			}
		}

		b2Body *bodyA = contact.fixtureA->GetBody();
		b2Body *bodyB = contact.fixtureB->GetBody();
		if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) 
		{
			CMyBodyBase *spriteA = (CMyBodyBase *) bodyA->GetUserData();
			CMyBodyBase *spriteB = (CMyBodyBase *) bodyB->GetUserData();

			// Sprite A = ball, Sprite B = Block
			if (spriteA->GetBodyType() == eBodyType_Ball && spriteB->GetBodyType() == eBodyType_Block) 
			{
				if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end()) 
				{
					toDestroy.push_back(bodyB);
				}
			}
			// Sprite B = block, Sprite A = ball
			else if (spriteA->GetBodyType() == eBodyType_Block && spriteB->GetBodyType() == eBodyType_Ball) 
			{
				if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end()) 
				{
					toDestroy.push_back(bodyA);
				}
			}        
		}     
	}

	std::vector<b2Body *>::iterator pos2;
	for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2)
	{
		b2Body *body = *pos2;     
		if (body->GetUserData() != NULL) 
		{
			CMyBodyBase *sprite = (CMyBodyBase *) body->GetUserData();
			guiex::CGUIWidgetManager::Instance()->DestroyWidget( sprite );
			m_arrayBlocks.erase( sprite );
			if( m_arrayBlocks.empty() )
			{
				CGUIFrameworkTest::ms_pFrameWork->BeginOpenMenuLayer("scene_win.xml");
				SetDisable( true );
				m_pPhysics->PauseSimulate();
				return;
			}
		}
	}
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_GameLayer::DestroySelf( )
{
	delete this;
}
//------------------------------------------------------------------------------



//*****************************************************************************
//	CMyCanvasLayer_MenuLayer
//*****************************************************************************
//------------------------------------------------------------------------------
CMyCanvasLayer_MenuLayer::CMyCanvasLayer_MenuLayer( const char* szLayerName, const char* szLayerConfigName )
:CGUICanvasLayer( szLayerName )
,m_pMenuRoot(NULL)
,m_strLayerConfigName(szLayerConfigName)
{

}
//------------------------------------------------------------------------------
CMyCanvasLayer_MenuLayer::~CMyCanvasLayer_MenuLayer(  )
{

}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MenuLayer::Initialize( )
{
	guiex::CGUICanvasLayer::Initialize();

	//load background widget
	m_pMenuRoot = guiex::CGUIWidgetManager::Instance()->LoadPage( m_strLayerConfigName, "game_breakout" );
	m_pMenuRoot->SetParent( this );

	m_pMenuRoot->Open();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MenuLayer::Finalize( )
{
	if( m_pMenuRoot )
	{
		guiex::CGUIWidgetManager::Instance()->DestroyWidget( m_pMenuRoot );
		m_pMenuRoot = NULL;
	}
	guiex::CGUICanvasLayer::Finalize();
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_MenuLayer::DestroySelf( )
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
:guiex::CGUIFramework(  )
,m_pGameLayer(NULL)
,m_pMenuLayer(NULL)
{
	ms_pFrameWork = this;
}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::CreateGameLayer( )
{
	if( !m_pGameLayer )
	{
		//create layer
		m_pGameLayer = new CMyCanvasLayer_GameLayer( "breakout game layer" );
		m_pGameLayer->Initialize();
		guiex::CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pGameLayer );
	}
}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::DestroyGameLayer( )
{
	if( m_pGameLayer )
	{
		guiex::CGUICanvasLayerManager::Instance()->DelayedDestroyCanvasLayer( m_pGameLayer );
		m_pGameLayer = NULL;
	}
}
//------------------------------------------------------------------------------
CMyCanvasLayer_GameLayer* CGUIFrameworkTest::GetGameLayer()
{
	return m_pGameLayer;
}
//------------------------------------------------------------------------------
CMyCanvasLayer_MenuLayer* CGUIFrameworkTest::GetMenuLayer()
{
	return m_pMenuLayer;
}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::CreateMenuLayer( const char* szLayerConfigName )
{
	if( !m_pMenuLayer )
	{
		//create layer
		m_pMenuLayer = new CMyCanvasLayer_MenuLayer( "breakout menu layer", szLayerConfigName );
		m_pMenuLayer->Initialize();
		guiex::CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pMenuLayer );

		if( guiex::CGUIString("scene_lose.xml") == szLayerConfigName )
		{
			guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->GetWidget( "btn_replay_lose", "game_breakout" );
			pWidget->RegisterNativeCallbackFunc( "OnMouseLeftClick", Btn_OnClick_Replay );
		}
		else if( guiex::CGUIString("scene_win.xml") == szLayerConfigName )
		{
			guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->GetWidget( "btn_replay_win", "game_breakout" );
			pWidget->RegisterNativeCallbackFunc( "OnMouseLeftClick", Btn_OnClick_Replay );
		}
		else if( guiex::CGUIString("scene_start.xml") == szLayerConfigName )
		{
			guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->GetWidget( "btn_start", "game_breakout" );
			pWidget->RegisterNativeCallbackFunc( "OnMouseLeftClick", Btn_OnClick_Replay );
		}
	}
}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::DestroyMenuLayer( )
{
	if( m_pMenuLayer )
	{
		guiex::CGUICanvasLayerManager::Instance()->DelayedDestroyCanvasLayer( m_pMenuLayer );
		m_pMenuLayer = NULL;
	}
}
//------------------------------------------------------------------------------
guiex::int32 CGUIFrameworkTest::InitializeGame()
{
	guiex::CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");

	guiex::CGUISceneManager::Instance()->LoadResources( "common" );	
	guiex::CGUISceneManager::Instance()->LoadWidgets( "common" );

	guiex::CGUISceneManager::Instance()->LoadResources( "game_breakout" );	

	//add utility widget
	guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	guiex::GSystem->GetUICanvas()->OpenUIPage(pWidget);		

	CreateMenuLayer("scene_start.xml");
	return 0;
}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::BeginOpenMenuLayer( const char* szLayerConfigName )
{
	CGUIFrameworkTest::ms_pFrameWork->CreateMenuLayer( szLayerConfigName );
	m_pMenuLayer->SetDisable( true );

	guiex::CGUIAsCallFunc* pAsCallFunc = guiex::CGUIAsManager::Instance()->AllocateResource<guiex::CGUIAsCallFunc>();
	pAsCallFunc->SetReceiver( m_pMenuLayer );
	pAsCallFunc->SetFuncCallback( CGUIFrameworkTest::FunCallback_DestroyGameLayer );

	guiex::CGUIAsFadeOutDownTiles* pAs = guiex::CGUIAsManager::Instance()->AllocateResource<guiex::CGUIAsFadeOutDownTiles>();
	pAs->SetGridSize( guiex::CGUIIntSize(30, 30) );
	pAs->SetReceiver( m_pMenuLayer );
	pAs->SetTotalTime( 3.0f );
	pAs->AddSuccessor( pAsCallFunc );
	pAs->SetReverse( true );

	m_pMenuLayer->SetAs( "effect", pAs );
	pAsCallFunc->RefRelease();
	pAs->RefRelease();

	m_pMenuLayer->PlayAs( pAs );

}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::FunCallback_DestroyGameLayer(guiex::CGUIAs* pAs)
{
	ms_pFrameWork->DestroyGameLayer();
	ms_pFrameWork->GetMenuLayer()->SetDisable( false );
}
//------------------------------------------------------------------------------
void CGUIFrameworkTest::Btn_OnClick_Replay( guiex::CGUIEventMouse* pEvent )
{
	ms_pFrameWork->DestroyMenuLayer();
	ms_pFrameWork->CreateGameLayer();
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<guiex::IGUIPhysics_box2d>();
	pPhysics->BeginSimulate();
}
//------------------------------------------------------------------------------



