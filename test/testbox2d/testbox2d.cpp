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

guiex::CGUIWidget* SampleInitialize()
{
	//create world
	b2Vec2 gravity( 0.0f, 10.0f );
	bool bDoSleep = true;
	g_pWorld = new b2World( gravity, bDoSleep );

	//create static body
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_dynamicBody;
	groundBodyDef.position.Set( 0.0f, 10.0f );
	b2Body * groundBody = g_pWorld->CreateBody( &groundBodyDef );
	b2PolygonShape groundBox;
	groundBox.SetAsBox( 50.0f, 10.0f );
	groundBody->CreateFixture( &groundBox, 1.0f );

	//create dynamic body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 0.0f, 4.0f );
	g_body = g_pWorld->CreateBody( &bodyDef );
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 1.0f, 1.0f );
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	g_body->CreateFixture( &fixtureDef );

	guiex::CGUIWidget* pWidget_staticimage = GUI_CREATE_WIDGET("CGUIWgtStaticImage", "staticimage_0", "testproject");
	pWidget_staticimage->SetImage( "color_white", "color_white" );
	pWidget_staticimage->SetValue( "Image", "color_white" );
	pWidget_staticimage->SetSize( 50, 50 );
	pWidget_staticimage->SetPosition( 100, 100 );
	pWidget_staticimage->SetAnchorPoint( 0.5, 0.5 );
	pWidget_staticimage->Create();

	return pWidget_staticimage;
}

void SampleUpdate( float fDeltaTime)
{
	int32 velocityIterations = 10;
	int32 positionIterations = 2;

	g_pWorld->Step( fDeltaTime, velocityIterations, positionIterations );
	g_pWorld->ClearForces();
	b2Vec2 position = g_body->GetPosition();
	float32 angle = g_body->GetAngle();
	guiex::CGUILogMsgManager::Instance()->Msg()->Log( 
		guiex::GUI_LM_DEBUG, 
		"Box2D: %4.2f %4.2f %4.2f",
		position.x,
		position.y,
		angle );
}

void SampleDestroy()
{
	if( g_pWorld )
	{
		delete g_pWorld;
		g_pWorld = NULL;
	}

}
