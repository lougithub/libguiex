/** 
* @file guiwgtbox2ddynamicbody.cpp
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbox2ddynamicbody.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_module/physics_box2d/guiphysics_box2d.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtBox2DDynamicBody);
	//------------------------------------------------------------------------------
	CGUIWgtBox2DDynamicBody::CGUIWgtBox2DDynamicBody( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBodyBase(StaticGetType(), rName, rSceneName)
	{
		InitBox2DDynamicBody();
	}
	//------------------------------------------------------------------------------
	CGUIWgtBox2DDynamicBody::CGUIWgtBox2DDynamicBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBodyBase(rType, rName, rSceneName)
	{
		InitBox2DDynamicBody();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::InitBox2DDynamicBody()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::UpdateBox2D()
	{
		CGUIWgtBox2DBase::UpdateBox2D();

		const b2Vec2& rPos = m_pBody->GetPosition();
		CGUIVector2 aPos( IGUIPhysics_box2d::Meter2Pixel(rPos.x), IGUIPhysics_box2d::Meter2Pixel(rPos.y) );
		real fRot = m_pBody->GetAngle() / b2_pi * 180.f;

		SetGlobalPixelPosition( aPos );
		SetRotation( 0, 0, fRot );
		Refresh();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::InitializeBox2D()
	{
		CGUIWgtBox2DBase::InitializeBox2D();

		GUI_ASSERT( m_pBody == NULL, "invalid static body pointer");

		b2World * pWorld = CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<IGUIPhysics_box2d>()->GetWorld();
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

		//create body
		b2BodyDef aBodyDef;
		aBodyDef.type = b2_dynamicBody;
		aBodyDef.position.Set( aPos.x, aPos.y );
		aBodyDef.angle = fRot;
		aBodyDef.bullet = true;
		m_pBody = pWorld->CreateBody( &aBodyDef );

		//create fixture
		b2PolygonShape aBox;
		aBox.SetAsBox( aSize.m_fWidth, aSize.m_fHeight, b2Vec2( aCenter.x, aCenter.y), 0.0f );
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &aBox;
		fixtureDef.density = 0.0f;
		fixtureDef.friction = 0.0f;
		m_pBody->CreateFixture( &fixtureDef );

		//set user data
		m_pBody->SetUserData( this );
	}
	//------------------------------------------------------------------------------

}//namespace guiex

