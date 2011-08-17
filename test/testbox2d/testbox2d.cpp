/** 
 * @file sample_opengl.cpp
 * @brief test and show how to use opengl as render

 * @author ken
 * @date 2006-07-11
 */

#include <libguiex_framework/guiframework.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <Box2D/Box2D.h>

using namespace guiex;

#define METER2PIXEL(meter)	((meter)*10.0f)

class CGUIFrameworkTest : public CGUIFramework
{
	public:
		CGUIFrameworkTest( )
			:CGUIFramework( )
		,m_pWorld( NULL )
		,m_pBody( NULL )
		,m_pBody2( NULL )
		,m_pJoint( NULL )
		,m_pWidgetRoot( NULL )
		{
		}
	protected:
		virtual guiex::int32 InitializeGame( );
		virtual void ReleaseGame( );

		virtual void PostRender( );

		void CreateBox2dSample_hellobox2d();
		void CreateBox2dSample_joints();
		void UpdateJoint( b2Joint* pJoint );
		void UpdateBody( b2Body* pBody );

	protected:
		b2World* m_pWorld;
		b2Body* m_pBody;
		b2Body* m_pBody2;
		b2Joint* m_pJoint;
		CGUIWidget* m_pWidgetRoot;
};

CGUIFrameworkBase* GUIEXCreateFramework( )
{
	return new CGUIFrameworkTest( );
}

const char* GUIEXGetDataDir()
{
	//should relative to dir "data"
	return "test";
}

guiex::int32 CGUIFrameworkTest::InitializeGame( )
{	
	CGUISceneManager::Instance()->RegisterScenesFromDir( "/", ".uip" );
	CGUISceneManager::Instance()->LoadResources( "common" );
	CGUISceneManager::Instance()->LoadWidgets( "common" );

	//create empty node for widget system
	m_pWidgetRoot = CGUIWidgetManager::Instance()->CreateWidget<CGUIWgtEmptyNode>( CGUIUtility::GenerateWidgetName(), "testproject");
	{
		m_pWidgetRoot->SetPositionType( eScreenValue_Percentage );
		m_pWidgetRoot->SetPosition( 0,0 );
		m_pWidgetRoot->Create();
	}

	//create world
	m_pWorld = CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<IGUIPhysics_box2d>()->GetWorld();

	CreateBox2dSample_hellobox2d();
	CreateBox2dSample_joints();

	CGUIWidgetManager::Instance()->AddPage( m_pWidgetRoot );
	GSystem->GetUICanvas()->OpenUIPage( m_pWidgetRoot );	
	CGUIWidget* pWidget = CGUIWidgetManager::Instance()->GetPage( "utility.xml", "common" );
	GSystem->GetUICanvas()->OpenUIPage(pWidget);	
	return 0;
}

void CGUIFrameworkTest::ReleaseGame( )
{
	m_pWorld = NULL;
}

void CGUIFrameworkTest::PostRender( )
{
	UpdateBody(m_pBody);
	UpdateBody(m_pBody2);

	UpdateJoint( m_pJoint );

	CGUIFramework::PostRender();
}

void CGUIFrameworkTest::CreateBox2dSample_hellobox2d()
{
	//create static body
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set( 40, 80 );
	b2Body * groundBody = m_pWorld->CreateBody( &groundBodyDef );
	b2PolygonShape groundBox;
	groundBox.SetAsBox( 80.0f, 4.0f );
	groundBody->CreateFixture( &groundBox, 0.0f );

	//widget
	CGUIWidget* pWidget_staticbody= GUI_CREATE_WIDGET("CGUIWgtStaticImage", CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_staticbody->SetParent( m_pWidgetRoot );
		pWidget_staticbody->SetImage( "bg", "color_white" );
		const b2Fixture* pFixtureList = groundBody->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		pWidget_staticbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_staticbody->SetPosition( METER2PIXEL(groundBody->GetPosition().x), METER2PIXEL(groundBody->GetPosition().y) );
		pWidget_staticbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_staticbody->SetColor( CGUIColor(0.0f, 0.0f, 1.0f, 1.0f ));
		pWidget_staticbody->Create();
	}

	//create dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 20.0f, 0.0f );
	bodyDef.angle = 0.25f*b2_pi;
	bodyDef.bullet = true;
	m_pBody = m_pWorld->CreateBody( &bodyDef );
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 4.0f, 1.0f );
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_pBody->CreateFixture( &fixtureDef );

	//widget
	CGUIWidget* pWidget_dynamicbody = GUI_CREATE_WIDGET("CGUIWgtStaticImage", CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_dynamicbody->SetParent( m_pWidgetRoot );
		pWidget_dynamicbody->SetImage( "bg", "color_white" );
		const b2Fixture* pFixtureList = m_pBody->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		//pWidget_dynamicbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_dynamicbody->SetSize( METER2PIXEL(4), METER2PIXEL(1) );
		pWidget_dynamicbody->SetRotation( 0, 0, m_pBody->GetAngle() / b2_pi * 180.f );
		pWidget_dynamicbody->SetPosition( METER2PIXEL(m_pBody->GetPosition().x), METER2PIXEL(m_pBody->GetPosition().y) );
		pWidget_dynamicbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_dynamicbody->Create();
	}
	m_pBody->SetUserData( pWidget_dynamicbody );
}

void CGUIFrameworkTest::CreateBox2dSample_joints()
{
	//create static body
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set( 40, 0 );
	b2Body * groundBody = m_pWorld->CreateBody( &groundBodyDef );
	b2PolygonShape groundBox;
	groundBox.SetAsBox( 8.0f, 8.0f );
	groundBody->CreateFixture( &groundBox, 0.0f );

	//widget
	CGUIWidget* pWidget_staticbody= GUI_CREATE_WIDGET("CGUIWgtStaticImage", CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_staticbody->SetParent( m_pWidgetRoot );
		pWidget_staticbody->SetImage( "bg", "color_white" );
		const b2Fixture* pFixtureList = groundBody->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		pWidget_staticbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_staticbody->SetPosition( METER2PIXEL(groundBody->GetPosition().x), METER2PIXEL(groundBody->GetPosition().y) );
		pWidget_staticbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_staticbody->SetColor( CGUIColor(0.0f, 0.0f, 1.0f, 1.0f ));
		pWidget_staticbody->Create();
	}

	//create dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 60.0f, 4.0f );
	bodyDef.bullet = true;
	m_pBody2 = m_pWorld->CreateBody( &bodyDef );
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 2.0f, 2.0f );
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	m_pBody2->CreateFixture( &fixtureDef );

	//widget
	CGUIWgtStaticImage* pWidget_dynamicbody = CGUIWidgetManager::Instance() ->CreateWidget<CGUIWgtStaticImage>(CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_dynamicbody->SetParent( m_pWidgetRoot );
		pWidget_dynamicbody->SetImage( "bg", "color_white" );
		const b2Fixture* pFixtureList = m_pBody2->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		//pWidget_dynamicbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_dynamicbody->SetSize( METER2PIXEL(2), METER2PIXEL(2) );
		pWidget_dynamicbody->SetRotation( 0, 0, m_pBody2->GetAngle() / b2_pi * 180.f );
		pWidget_dynamicbody->SetPosition( METER2PIXEL(m_pBody2->GetPosition().x), METER2PIXEL(m_pBody2->GetPosition().y) );
		pWidget_dynamicbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_dynamicbody->Create();
	}
	m_pBody2->SetUserData( pWidget_dynamicbody );

	//joint
	b2DistanceJointDef jointDef;
	jointDef.bodyA = groundBody;
	jointDef.bodyB = m_pBody2;
	jointDef.localAnchorA = jointDef.bodyA->GetLocalCenter();
	jointDef.localAnchorB = jointDef.bodyB->GetLocalCenter();
	jointDef.length = 30;
	m_pJoint = m_pWorld->CreateJoint( &jointDef );
}



void CGUIFrameworkTest::UpdateBody( b2Body* pBody )
{
	CGUIWidget* pBodyWidget = (CGUIWidget*)(pBody->GetUserData());
	pBodyWidget->SetPixelPosition( METER2PIXEL(pBody->GetPosition().x), METER2PIXEL(pBody->GetPosition().y) );
	pBodyWidget->SetRotation( 0, 0, pBody->GetAngle() / b2_pi * 180.f );
	pBodyWidget->Refresh();
}

void CGUIFrameworkTest::UpdateJoint( b2Joint* pJoint )
{
	IGUIInterfaceRender* pRender = CGUIInterfaceManager::Instance()->GetInterfaceRender();
	pRender->BeginRender();
	CGUIVector2 aBegin( METER2PIXEL(m_pJoint->GetAnchorA().x), METER2PIXEL(m_pJoint->GetAnchorA().y ));
	CGUIVector2 aEnd( METER2PIXEL(m_pJoint->GetAnchorB().x), METER2PIXEL(m_pJoint->GetAnchorB().y ));
	pRender->DrawLine( aBegin, aEnd, 2, 0, CGUIColor(1,0,0,1), CGUIColor( 1,0,0,1) );
	pRender->EndRender();
}

