/** 
* @file guimusicdata_cocoa.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata_cocoa.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData_cocoa::CGUIMusicData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUIMusicData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData_cocoa::~CGUIMusicData_cocoa()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicData_cocoa::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData_cocoa::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
