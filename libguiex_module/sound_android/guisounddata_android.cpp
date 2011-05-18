/** 
* @file guisounddata_android.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-10
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_android.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_android::CGUISoundData_android( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_android::~CGUISoundData_android()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_android::DoLoad()
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_android::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
