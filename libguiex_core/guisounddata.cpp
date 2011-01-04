/** 
* @file guisounddata.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-01-04
*/

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guibase.h>
#include <libguiex_core/guisounddata.h>
#include <libguiex_core/guiinterfacesound.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiexception.h>
#include <libguiex_core/guiscene.h>
#include <libguiex_core/guiscenemanager.h>



//============================================================================//
// function
//============================================================================//


namespace guiex
{

	//------------------------------------------------------------------------------
	CGUISoundData::CGUISoundData( 
		const CGUIString& rName,
		const CGUIString& rSceneName, 
		uint32 nSoundID)
		:CGUIResource( rName, rSceneName, "SOUND" )
		,m_nSoundID( nSoundID )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData::~CGUISoundData()
	{
	}
	//------------------------------------------------------------------------------
	uint32 CGUISoundData::GetSoundID() const
	{
		return m_nSoundID;
	}
	//------------------------------------------------------------------------------
}
