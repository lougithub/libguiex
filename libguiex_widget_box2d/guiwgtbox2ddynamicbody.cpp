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
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DDynamicBody::OnCreate()
	{
		CGUIWgtBox2DBase::OnCreate();

		//create static body
		//b2BodyDef groundBodyDef;
		//groundBodyDef.type = b2_staticBody;
		//groundBodyDef.position.Set( 40, 80 );
		//b2Body * groundBody = m_pWorld->CreateBody( &groundBodyDef );
		//b2PolygonShape groundBox;
		//groundBox.SetAsBox( 80.0f, 4.0f );
		//groundBody->CreateFixture( &groundBox, 0.0f );
	}
	//------------------------------------------------------------------------------

}//namespace guiex

