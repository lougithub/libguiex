/** 
* @file guisounddata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_openal.h"


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_openal::CGUISoundData_openal( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nSoundID )
		:CGUISoundData( rName, rSceneName, nSoundID )
		,m_strPath( rPath )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_openal::~CGUISoundData_openal()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_openal::DoLoad() const
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_openal::DoUnload()
	{

	}
	//------------------------------------------------------------------------------
}
