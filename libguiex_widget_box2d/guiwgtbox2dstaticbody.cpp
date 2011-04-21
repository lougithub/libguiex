/** 
* @file guiwgtbox2dstaticbody.cpp
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbox2dstaticbody.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>
#include <Box2D/Box2D.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtBox2DStaticBody);
	//------------------------------------------------------------------------------
	CGUIWgtBox2DStaticBody::CGUIWgtBox2DStaticBody( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBodyBase(StaticGetType(), rName, rSceneName)
	{
		InitBox2DStaticBody();
	}
	//------------------------------------------------------------------------------
	CGUIWgtBox2DStaticBody::CGUIWgtBox2DStaticBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBodyBase(rType, rName, rSceneName)
	{
		InitBox2DStaticBody();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::InitBox2DStaticBody()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::InitializeBox2D()
	{
		CGUIWgtBox2DBase::InitializeBox2D();

		GUI_ASSERT( m_pBody == NULL, "invalid static body pointer");

		b2World * pWorld = CGUIInterfaceManager::Instance()->GetInterfacePhysicsWithTypeCheck<IGUIPhysics_box2d>()->GetWorld();
		if( !pWorld )
		{
			return;
		}

		//get position,size and so on
		CGUIVector2 aPos;
		CGUIVector2 aCenter;
		CGUISize aSize;
		real fRot = 0.0f;
		GetBox2dPosition( aPos );
		GetBox2dSize( aSize );
		GetBox2dCenter( aCenter );
		GetBox2dRot( fRot );

		//create static body
		b2BodyDef aBodyDef;
		aBodyDef.type = b2_staticBody;
		aBodyDef.position.Set( aPos.x, aPos.y );
		aBodyDef.angle = fRot;
		m_pBody = pWorld->CreateBody(&aBodyDef);

		//create fixture
		b2PolygonShape aBox;
		aBox.SetAsBox( aSize.m_fWidth, aSize.m_fHeight, b2Vec2( aCenter.x, aCenter.y), 0.0f );
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &aBox;
		fixtureDef.density = 0.0f;
		m_pBody->CreateFixture( &fixtureDef );

		//set user data
		m_pBody->SetUserData( this );
	}
	//------------------------------------------------------------------------------
}//namespace guiex

