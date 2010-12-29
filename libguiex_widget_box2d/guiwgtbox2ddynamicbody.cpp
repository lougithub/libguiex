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
		:CGUIWgtBox2DBase(StaticGetType(), rName, rSceneName)
	{
		InitBox2DDynamicBody();
	}
	//------------------------------------------------------------------------------
	CGUIWgtBox2DDynamicBody::CGUIWgtBox2DDynamicBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(rType, rName, rSceneName)
	{
		InitBox2DDynamicBody();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::InitBox2DDynamicBody()
	{
		m_pDynamicBody = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::OnCreate()
	{
		CGUIWgtBox2DBase::OnCreate();

		InitializeBox2D();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::OnUpdate()
	{
		CGUIWgtBox2DBase::OnUpdate();

		//const b2Vec2& rPos = m_pDynamicBody->GetPosition();
		//real fRot = m_pDynamicBody->GetAngle() / b2_pi * 180.f;

		//SetPixelPosition( IGUIPhysics_box2d::Meter2Pixel(rPos.x), IGUIPhysics_box2d::Meter2Pixel(rPos.y) );
		//SetRotation( 0, 0, fRot );
		//Refresh();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::InitializeBox2D()
	{
		GUI_ASSERT( m_pDynamicBody == NULL, "invalid static body pointer");

		b2World * pWorld = CGUIInterfaceManager::Instance()->GetInterfacePhysicsTyped<IGUIPhysics_box2d>()->GetWorld();
		if( !pWorld )
		{
			return;
		}

		//get position,size and so on
		CGUIVector2 vPos = GetPosition();
		LocalToWorld( vPos );
		const CGUISize& rSize = GetPixelSize();
		real fRot = GetRotation().z / 180.f * b2_pi;

		//create body
		b2BodyDef aBodyDef;
		aBodyDef.type = b2_dynamicBody;
		aBodyDef.position.Set( IGUIPhysics_box2d::Pixel2Meter(vPos.x), IGUIPhysics_box2d::Pixel2Meter(vPos.y) );
		aBodyDef.angle = fRot;
		aBodyDef.bullet = true;
		m_pDynamicBody = pWorld->CreateBody( &aBodyDef );

		//create fixture
		b2PolygonShape aBox;
		aBox.SetAsBox( rSize.GetWidth(), rSize.GetHeight() );
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &aBox;
		fixtureDef.density = 0.0f;
		fixtureDef.friction = 0.0f;
		m_pDynamicBody->CreateFixture( &fixtureDef );

		//set user data
		m_pDynamicBody->SetUserData( this );
	}
	//------------------------------------------------------------------------------

}//namespace guiex

