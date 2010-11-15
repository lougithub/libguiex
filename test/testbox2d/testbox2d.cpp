/** 
 * @file sample_opengl.cpp
 * @brief test and show how to use opengl as render

 * @author ken
 * @date 2006-07-11
 */

#include <libguiex_core/guiex.h>
#include <libguiex_widget/guiwgt.h>
#include <Box2D/Box2D.h>

b2World * g_pWorld = NULL;
b2Body* g_body = NULL;
b2Body* g_body2 = NULL;
b2Joint* g_pJoint = NULL;
guiex::CGUIWidget* g_pWidgetRoot = NULL;

#define METER2PIXEL(meter)	((meter)*10.0f)

void CreateBox2dSample_hellobox2d()
{
	//create static body
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set( 40, 80 );
	b2Body * groundBody = g_pWorld->CreateBody( &groundBodyDef );
	b2PolygonShape groundBox;
	groundBox.SetAsBox( 80.0f, 4.0f );
	groundBody->CreateFixture( &groundBox, 0.0f );

	//widget
	guiex::CGUIWidget* pWidget_staticbody= GUI_CREATE_WIDGET("CGUIWgtStaticImage", guiex::CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_staticbody->SetParent( g_pWidgetRoot );
		pWidget_staticbody->SetImage( "color_white", "color_white" );
		pWidget_staticbody->SetValue( "Image", "color_white" );
		const b2Fixture* pFixtureList = groundBody->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		pWidget_staticbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_staticbody->SetPosition( METER2PIXEL(groundBody->GetPosition().x), METER2PIXEL(groundBody->GetPosition().y) );
		pWidget_staticbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_staticbody->SetColor( guiex::CGUIColor(0.0f, 0.0f, 1.0f, 1.0f ));
		pWidget_staticbody->Create();
	}

	//create dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 20.0f, 0.0f );
	bodyDef.angle = 0.25f*b2_pi;
	bodyDef.bullet = true;
	g_body = g_pWorld->CreateBody( &bodyDef );
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 4.0f, 1.0f );
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	g_body->CreateFixture( &fixtureDef );

	//widget
	guiex::CGUIWidget* pWidget_dynamicbody = GUI_CREATE_WIDGET("CGUIWgtStaticImage", guiex::CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_dynamicbody->SetParent( g_pWidgetRoot );
		pWidget_dynamicbody->SetImage( "color_white", "color_white" );
		pWidget_dynamicbody->SetValue( "Image", "color_white" );
		const b2Fixture* pFixtureList = g_body->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		//pWidget_dynamicbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_dynamicbody->SetSize( METER2PIXEL(4), METER2PIXEL(1) );
		pWidget_dynamicbody->SetRotation( 0, 0, g_body->GetAngle() / b2_pi * 180.f );
		pWidget_dynamicbody->SetPosition( METER2PIXEL(g_body->GetPosition().x), METER2PIXEL(g_body->GetPosition().y) );
		pWidget_dynamicbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_dynamicbody->Create();
	}
	g_body->SetUserData( pWidget_dynamicbody );
}

void CreateBox2dSample_joints()
{
	//create static body
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set( 40, 0 );
	b2Body * groundBody = g_pWorld->CreateBody( &groundBodyDef );
	b2PolygonShape groundBox;
	groundBox.SetAsBox( 8.0f, 8.0f );
	groundBody->CreateFixture( &groundBox, 0.0f );

	//widget
	guiex::CGUIWidget* pWidget_staticbody= GUI_CREATE_WIDGET("CGUIWgtStaticImage", guiex::CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_staticbody->SetParent( g_pWidgetRoot );
		pWidget_staticbody->SetImage( "color_white", "color_white" );
		pWidget_staticbody->SetValue( "Image", "color_white" );
		const b2Fixture* pFixtureList = groundBody->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		pWidget_staticbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_staticbody->SetPosition( METER2PIXEL(groundBody->GetPosition().x), METER2PIXEL(groundBody->GetPosition().y) );
		pWidget_staticbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_staticbody->SetColor( guiex::CGUIColor(0.0f, 0.0f, 1.0f, 1.0f ));
		pWidget_staticbody->Create();
	}

	//create dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 60.0f, 4.0f );
	bodyDef.bullet = true;
	g_body2 = g_pWorld->CreateBody( &bodyDef );
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 2.0f, 2.0f );
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	g_body2->CreateFixture( &fixtureDef );

	//widget
	guiex::CGUIWidget* pWidget_dynamicbody = GUI_CREATE_WIDGET("CGUIWgtStaticImage", guiex::CGUIUtility::GenerateWidgetName(), "testproject");
	{
		pWidget_dynamicbody->SetParent( g_pWidgetRoot );
		pWidget_dynamicbody->SetImage( "color_white", "color_white" );
		pWidget_dynamicbody->SetValue( "Image", "color_white" );
		const b2Fixture* pFixtureList = g_body2->GetFixtureList();
		b2Vec2 vExtents = pFixtureList->GetAABB().GetExtents();
		//pWidget_dynamicbody->SetSize( METER2PIXEL(vExtents.x*2), METER2PIXEL(vExtents.y*2) );
		pWidget_dynamicbody->SetSize( METER2PIXEL(2), METER2PIXEL(2) );
		pWidget_dynamicbody->SetRotation( 0, 0, g_body2->GetAngle() / b2_pi * 180.f );
		pWidget_dynamicbody->SetPosition( METER2PIXEL(g_body2->GetPosition().x), METER2PIXEL(g_body2->GetPosition().y) );
		pWidget_dynamicbody->SetAnchorPoint( 0.5, 0.5 );
		pWidget_dynamicbody->Create();
	}
	g_body2->SetUserData( pWidget_dynamicbody );

	//joint
	b2DistanceJointDef jointDef;
	jointDef.bodyA = groundBody;
	jointDef.bodyB = g_body2;
	jointDef.localAnchorA = jointDef.bodyA->GetLocalCenter();
	jointDef.localAnchorB = jointDef.bodyB->GetLocalCenter();
	jointDef.length = 30;
	g_pJoint = g_pWorld->CreateJoint( &jointDef );
}

guiex::CGUIWidget* SampleInitialize()
{
	//create empty node for widget system
	g_pWidgetRoot = GUI_CREATE_WIDGET("CGUIWgtEmptyNode", guiex::CGUIUtility::GenerateWidgetName(), "testproject");
	{
		g_pWidgetRoot->SetPositionType( guiex::eScreenValue_Percentage );
		g_pWidgetRoot->SetPosition( 0,0 );
		g_pWidgetRoot->Create();
	}

	//create world
	b2Vec2 gravity( 0.0f, 10.0f );
	bool bDoSleep = true;
	g_pWorld = new b2World( gravity, bDoSleep );

	CreateBox2dSample_hellobox2d();
	CreateBox2dSample_joints();

	return g_pWidgetRoot;
}

void UpdateBody( b2Body* pBody )
{
	guiex::CGUIWidget* pBodyWidget = (guiex::CGUIWidget*)(pBody->GetUserData());
	pBodyWidget->SetPixelPosition( METER2PIXEL(pBody->GetPosition().x), METER2PIXEL(pBody->GetPosition().y) );
	pBodyWidget->SetRotation( 0, 0, pBody->GetAngle() / b2_pi * 180.f );
	pBodyWidget->Refresh();
}

void UpdateJoint( b2Joint* pJoint )
{
	guiex::IGUIInterfaceRender* pRender = guiex::CGUIInterfaceManager::Instance()->GetInterfaceRender();
	pRender->BeginRender();
	guiex::CGUIVector2 aBegin( METER2PIXEL(g_pJoint->GetAnchorA().x), METER2PIXEL(g_pJoint->GetAnchorA().y ));
	guiex::CGUIVector2 aEnd( METER2PIXEL(g_pJoint->GetAnchorB().x), METER2PIXEL(g_pJoint->GetAnchorB().y ));
	pRender->DrawLine( g_pWidgetRoot->GetFullTransform(), aBegin, aEnd, 2, pRender->GetAndIncZ(), guiex::CGUIColor(1,0,0,1).GetRGBA(), guiex::CGUIColor( 1,0,0,1).GetRGBA() );
	pRender->EndRender();
}

void SampleUpdate( float fDeltaTime)
{
	int32 velocityIterations = 10;
	int32 positionIterations = 2;

	g_pWorld->Step( fDeltaTime, velocityIterations, positionIterations );
	g_pWorld->ClearForces();

	UpdateBody(g_body);
	UpdateBody(g_body2);

	UpdateJoint( g_pJoint );
}

void SampleDestroy()
{
	if( g_pWorld )
	{
		delete g_pWorld;
		g_pWorld = NULL;
	}

}
