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
	CGUISoundData::CGUISoundData( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "SOUND" )
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData::~CGUISoundData()
	{
	}
	//------------------------------------------------------------------------------
	void CGUISoundData::Play()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->Play( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISoundData::Stop()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->Stop( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISoundData::Pause()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->Pause( this );
		}
	}
	//------------------------------------------------------------------------------
	bool CGUISoundData::IsPlaying()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			return pInterfaceSound->IsPlaying( this );
		}
		return false;
	}
	//------------------------------------------------------------------------------
}
