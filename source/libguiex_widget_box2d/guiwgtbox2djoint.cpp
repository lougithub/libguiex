/** 
* @file guiwgtbox2djoint.cpp
* @brief use physic engine box2d
* @author ken
* @date 2010-12-21
*/

//============================================================================//
// include 
//============================================================================// 
#include "guiwgtbox2djoint.h"
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiimage.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtBox2DJoint);
	//------------------------------------------------------------------------------
	CGUIWgtBox2DJoint::CGUIWgtBox2DJoint( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(StaticGetType(), rName, rSceneName)
	{
		InitBox2DJoint();
	}
	//------------------------------------------------------------------------------
	CGUIWgtBox2DJoint::CGUIWgtBox2DJoint( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWgtBox2DBase(rType, rName, rSceneName)
	{
		InitBox2DJoint();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtBox2DJoint::InitBox2DJoint()
	{
	}
	//------------------------------------------------------------------------------
}//namespace guiex

