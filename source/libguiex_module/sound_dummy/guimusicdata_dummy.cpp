/** 
* @file guimusicdata_dummy.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata_dummy.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData_dummy::CGUIMusicData_dummy( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUIMusicData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData_dummy::~CGUIMusicData_dummy()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicData_dummy::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData_dummy::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
