/** 
* @file guisounddata_opensles.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-10
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_opensles.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_opensles::CGUISoundData_opensles( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_opensles::~CGUISoundData_opensles()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_opensles::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_opensles::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
