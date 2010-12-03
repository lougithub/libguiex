/** 
* @file guiwgtemptynode.cpp
* @brief empty node, used to manage widget
* for example, used to group radio button
* @author ken
* @date 2006-08-03
*/


//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtemptynode.h>
#include <libguiex_core/guiinterfacerender.h>
#include <libguiex_core/guiwidgetsystem.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_WIDGET_GENERATOR_IMPLEMENT(CGUIWgtEmptyNode);
	//------------------------------------------------------------------------------
	CGUIWgtEmptyNode::CGUIWgtEmptyNode(const CGUIString& rName, const CGUIString& rSceneName)
		:CGUIWidget(StaticGetType(), rName, rSceneName)
	{
		InitEmptyNode();
	}
	//------------------------------------------------------------------------------
	CGUIWgtEmptyNode::CGUIWgtEmptyNode( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitEmptyNode();
	}
	//------------------------------------------------------------------------------
	void	CGUIWgtEmptyNode::InitEmptyNode()
	{
		SetSizeType(eScreenValue_Percentage);
		SetSize( 1.0f, 1.0f );
		SetFocusable(false);
		SetActivable(false);
		
		SetMouseConsumed(false);
		SetHitable( false );				
	}
	//------------------------------------------------------------------------------
}//namespace guiex
