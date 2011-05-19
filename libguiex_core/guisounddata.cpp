/** 
* @file guisounddata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata.h"
#include "guiinterfacesound.h"
#include "guiinterfacemanager.h"
#include "guiexception.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guisystem.h"
#include "guisoundmanager.h"



//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData::CGUISoundData( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "SOUND", GSystem->GetSoundManager() )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData::~CGUISoundData()
	{
	}
	//------------------------------------------------------------------------------
}
