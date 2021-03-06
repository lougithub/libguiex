/** 
* @file guisounddata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata.h"
#include "guiinterfacesound.h"
#include "guiinterfacemanager.h"
#include "guiexception.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guisystem.h"
#include "guimusicmanager.h"



//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData::CGUIMusicData( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "MUSIC", GSystem->GetMusicManager() )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData::~CGUIMusicData()
	{
	}
	//------------------------------------------------------------------------------
}
