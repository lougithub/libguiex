/** 
* @file guimusicdata_opensles.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata_opensles.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData_opensles::CGUIMusicData_opensles( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUIMusicData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData_opensles::~CGUIMusicData_opensles()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicData_opensles::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData_opensles::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
