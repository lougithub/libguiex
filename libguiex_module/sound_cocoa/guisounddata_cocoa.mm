/** 
* @file guisounddata_cocoa.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_cocoa.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_cocoa::CGUISoundData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_cocoa::~CGUISoundData_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_cocoa::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_cocoa::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
