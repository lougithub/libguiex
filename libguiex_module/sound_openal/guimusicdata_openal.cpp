/** 
* @file guimusicdata_openal.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-06
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata_openal.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData_openal::CGUIMusicData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUIMusicData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData_openal::~CGUIMusicData_openal()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicData_openal::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData_openal::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
