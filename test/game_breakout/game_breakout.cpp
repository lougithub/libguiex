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
//	CMyBall
//*****************************************************************************
//------------------------------------------------------------------------------
GUI_CUSTOM_WIDGET_IMPLEMENT( CMyBall );
//------------------------------------------------------------------------------
CMyBall::CMyBall( const guiex::CGUIString& rName, const guiex::CGUIString& rSceneName )
:guiex::CGUIWgtStaticImage( StaticGetType(), rName, rSceneName )
,m_pBody(NULL)
,m_pFixture(NULL)
{

}
//------------------------------------------------------------------------------
void CMyBall::InitBall( guiex::CGUIWidget* pParent )
{
	SetParent( pParent );
	SetImage( "BGIMAGE", "Ball" );
	SetAnchorPoint( 0.5f, 0.5f );
	SetPixelPosition( 100, guiex::GSystem->GetScreenHeight() - 100.0f );
	Refresh();

	// Create ball body 
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<guiex::IGUIPhysics_box2d>();
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
	b2Vec2 force = b2Vec2(10, 10);
	m_pBody->ApplyLinearImpulse(force, ballBodyDef.position);
}
//------------------------------------------------------------------------------
b2Fixture* CMyBall::GetFixture()
{
	return m_pFixture;
}
//------------------------------------------------------------------------------
void CMyBall::OnUpdate(guiex::real fDeltaTime)
{
	guiex::CGUIWgtStaticImage::OnUpdate(fDeltaTime );

	const b2Vec2& rPos = m_pBody->GetPosition();
	guiex::CGUIVector2 aPos( guiex::IGUIPhysics_box2d::Meter2Pixel(rPos.x), guiex::IGUIPhysics_box2d::Meter2Pixel(rPos.y) );
	guiex::real fRot = m_pBody->GetAngle() / b2_pi * 180.f;

	SetPixelPosition( aPos );
	SetRotation( 0, 0, fRot );
	Refresh();

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
:guiex::CGUIWgtStaticImage( StaticGetType(), rName, rSceneName )
,m_pBody(NULL)
,m_pFixture(NULL)
,m_mouseJoint(NULL)
{

}
//------------------------------------------------------------------------------
void CMyPaddle::InitPaddle( guiex::CGUIWidget* pParent )
{
	SetParent( pParent );
	SetImage( "BGIMAGE", "Paddle" );
	SetHitable( true );
	SetMovable( true );
	SetAnchorPoint( 0.5f, 0.5f );
	SetPixelPosition( guiex::GSystem->GetScreenWidth() / 2.0f, guiex::GSystem->GetScreenHeight() - 50.0f );
	Refresh();

	// Create paddle body 
	guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<guiex::IGUIPhysics_box2d>();
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
	pPhysics->GetWorld()->CreateJoint(&jointDef);
}
//------------------------------------------------------------------------------
void CMyPaddle::OnUpdate( guiex::real fDeltaTime )
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
uint32 CMyPaddle::OnDragBegin( guiex::CGUIEventDrag* pEvent )
{
	if (m_mouseJoint != NULL) 
	{
		return 0;
	}

	b2Vec2 locationWorld = b2Vec2( guiex::IGUIPhysics_box2d::Pixel2Meter(pEvent->GetMouseGlobalPos().x), guiex::IGUIPhysics_box2d::Pixel2Meter(pEvent->GetMouseGlobalPos().y));

	if (m_pFixture->TestPoint(locationWorld)) 
	{
		guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<guiex::IGUIPhysics_box2d>();

		b2MouseJointDef md;
		md.bodyA = CGUIFrameworkTest::ms_pFrameWork->GetGameLayer()->GetGroundBody();
		md.bodyB = m_pBody;
		md.target = locationWorld;
		md.collideConnected = true;
		md.maxForce = 1000.0f * m_pBody->GetMass();

		m_mouseJoint = (b2MouseJoint *)pPhysics->GetWorld()->CreateJoint(&md);
		m_pBody->SetAwake(true);
	}

	return guiex::CGUIWgtStaticImage::OnDragBegin(pEvent);
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

	return guiex::CGUIWgtStaticImage::OnDragProcess(pEvent);
}
//------------------------------------------------------------------------------
uint32 CMyPaddle::OnDragEnd( guiex::CGUIEventDrag* pEvent )
{
	if (m_mouseJoint) 
	{
		guiex::IGUIPhysics_box2d* pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<guiex::IGUIPhysics_box2d>();
		pPhysics->GetWorld()->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
	}

	return guiex::CGUIWgtStaticImage::OnDragEnd(pEvent);
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
//	CMyCanvasLayer_BreakoutGame
//*****************************************************************************

//------------------------------------------------------------------------------
CMyCanvasLayer_BreakoutGame::CMyCanvasLayer_BreakoutGame( const char* szLayerName )
:CGUICanvasLayer( szLayerName )
,m_groundBody(NULL)
,m_bottomFixture(NULL)
,m_pPhysics(NULL)
,m_contactListener(NULL)
,m_pBall(NULL)
{
	//set attribute
	SetHitable( true );
	SetGenerateClickEvent( true );
	SetMouseConsumed(true);
	Refresh();

	//init physics
	m_pPhysics = guiex::CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<guiex::IGUIPhysics_box2d>();
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
CMyCanvasLayer_BreakoutGame::~CMyCanvasLayer_BreakoutGame(  )
{
	delete m_contactListener;
	m_contactListener = NULL;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_BreakoutGame::Initialize( )
{
	// Create sprite and add it to the layer
	m_pBall = guiex::CGUIWidgetManager::Instance()->CreateCustomWidget<CMyBall>( "ball", "" );
	m_pBall->InitBall( this );

	// Create paddle and add it to the layer
	CMyPaddle* pPaddle = guiex::CGUIWidgetManager::Instance()->CreateCustomWidget<CMyPaddle>( "paddle", "" );
	pPaddle->InitPaddle( this );

	guiex::CGUICanvasLayer::Initialize();
}
//------------------------------------------------------------------------------
b2Body* CMyCanvasLayer_BreakoutGame::GetGroundBody()
{
	return m_groundBody;
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_BreakoutGame::OnUpdate(guiex::real fDeltaTime)
{
	CGUICanvasLayer::OnUpdate(fDeltaTime );

	std::vector<MyContact>::iterator pos;
	for(pos = m_contactListener->_contacts.begin(); 
		pos != m_contactListener->_contacts.end(); ++pos)
	{
		MyContact contact = *pos;

		if ((contact.fixtureA == m_bottomFixture && contact.fixtureB == m_pBall->GetFixture()) ||
			(contact.fixtureA == m_pBall->GetFixture() && contact.fixtureB == m_bottomFixture)) 
		{
			GUI_TRACE( "Ball hit bottom!" );
		}
	}
}
//------------------------------------------------------------------------------
void CMyCanvasLayer_BreakoutGame::DestroySelf( )
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
{
	ms_pFrameWork = this;
}
//------------------------------------------------------------------------------
CMyCanvasLayer_BreakoutGame* CGUIFrameworkTest::GetGameLayer()
{
	return m_pGameLayer;
}
//------------------------------------------------------------------------------
guiex::int32 CGUIFrameworkTest::InitializeGame()
{
	guiex::CGUISceneManager::Instance()->RegisterScenesFromDir("/", ".uip");

	guiex::CGUISceneManager::Instance()->LoadResources( "common" );	
	guiex::CGUISceneManager::Instance()->LoadWidgets( "common" );

	guiex::CGUISceneManager::Instance()->LoadResources( "game_breakout" );	
	//guiex::CGUISceneManager::Instance()->LoadWidgets( "game_breakout" );

	//add utility widget
	guiex::CGUIWidget* pWidget = guiex::CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	guiex::GSystem->GetUICanvas()->OpenUIPage(pWidget);		

	//create layer
	m_pGameLayer = new CMyCanvasLayer_BreakoutGame( "breakout game layer" );
	m_pGameLayer->Initialize();
	guiex::CGUICanvasLayerManager::Instance()->PushCanvasLayer( m_pGameLayer );
	return 0;
}
//------------------------------------------------------------------------------




