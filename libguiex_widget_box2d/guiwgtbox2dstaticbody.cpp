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

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtBox2DStaticBody);
	//------------------------------------------------------------------------------
	CGUIWgtBox2DStaticBody::CGUIWgtBox2DStaticBody( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(StaticGetType(), rName, rSceneName)
	{
		InitBox2DStaticBody();
	}
	//------------------------------------------------------------------------------
	CGUIWgtBox2DStaticBody::CGUIWgtBox2DStaticBody( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(rType, rName, rSceneName)
	{
		InitBox2DStaticBody();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::InitBox2DStaticBody()
	{
		m_pStaticBody = NULL;
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::OnCreate()
	{
		CGUIWgtBox2DBase::OnCreate();

		InitializeBox2D();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DStaticBody::InitializeBox2D()
	{
		GUI_ASSERT( m_pStaticBody == NULL, "invalid static body pointer");

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

		//create static body
		b2BodyDef aBodyDef;
		aBodyDef.type = b2_staticBody;
		aBodyDef.position.Set( IGUIPhysics_box2d::Pixel2Meter(vPos.x), IGUIPhysics_box2d::Pixel2Meter(vPos.y) );
		aBodyDef.angle = fRot;
		m_pStaticBody = pWorld->CreateBody(&aBodyDef);

		//create fixture
		b2PolygonShape aBox;
		aBox.SetAsBox( rSize.GetWidth(), rSize.GetHeight() );
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &aBox;
		m_pStaticBody->CreateFixture( &fixtureDef );

		//set user data
		m_pStaticBody->SetUserData( this );
	}
	//------------------------------------------------------------------------------
}//namespace guiex

