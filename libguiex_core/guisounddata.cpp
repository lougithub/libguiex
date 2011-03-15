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
	void CGUISoundData::Play()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->PlayEffect( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISoundData::Stop()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->StopEffect( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISoundData::Pause()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->PauseEffect( this );
		}
	}
	//------------------------------------------------------------------------------
	bool CGUISoundData::IsPlaying()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			return pInterfaceSound->IsPlayingEffect( this );
		}
		return false;
	}
	//------------------------------------------------------------------------------
}
