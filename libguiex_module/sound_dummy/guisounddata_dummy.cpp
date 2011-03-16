/** 
* @file guisounddata_dummy.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_dummy.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_dummy::CGUISoundData_dummy( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_dummy::~CGUISoundData_dummy()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_dummy::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_dummy::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
