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



//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData::CGUIMusicData( const CGUIString& rName, const CGUIString& rSceneName )
		:CGUIResource( rName, rSceneName, "MUSIC" )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData::~CGUIMusicData()
	{
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData::Play()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->PlayMusic( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData::Stop()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->StopMusic( this );
		}
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData::Pause()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			pInterfaceSound->PauseMusic( this );
		}
	}
	//------------------------------------------------------------------------------
	bool CGUIMusicData::IsPlaying()
	{
		Load();

		IGUIInterfaceSound* pInterfaceSound = CGUIInterfaceManager::Instance()->GetInterfaceSound();
		if( pInterfaceSound )
		{
			return pInterfaceSound->IsPlayingMusic( this );
		}
		return false;
	}
	//------------------------------------------------------------------------------
}
