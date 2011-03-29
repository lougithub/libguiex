/** 
 * @file guiwgtstatic.cpp
 * @brief base class, for static widget
 * @author ken
 * @date 2006-07-19
 */

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_widget/guiwgtstatic.h>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIWgtStatic::CGUIWgtStatic( const CGUIString& rType, const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIWidget(rType, rName, rSceneName)
	{
		InitStatic();
	}
	//------------------------------------------------------------------------------
	void CGUIWgtStatic::InitStatic()
	{

	}
	//------------------------------------------------------------------------------
}//namespace guiex
