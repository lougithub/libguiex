/** 
* @file guiinterfacesound.cpp
* @brief sound interface
* @author ken
* @date 2006-08-31
*/

//============================================================================//
// include
//============================================================================// 
#include "guiinterfacesound.h"
#include "guiinterfacemanager.h"
#include "guisoundmanager.h"
#include "guimusicmanager.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------ 
	GUIEXPORT IGUIInterfaceSound* GetInterface_Sound( )
	{
		return CGUIInterfaceManager::Instance()->GetInterfaceSound();
	}
	//------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------------
	const char* IGUIInterfaceSound::StaticGetModuleType()
	{ 
		return "IGUISound";
	}
	//------------------------------------------------------------------------------
	IGUIInterfaceSound::IGUIInterfaceSound( const char* szModuleName )
		:IGUIInterface( StaticGetModuleType(), szModuleName )
	{

	}
	//------------------------------------------------------------------------------ 
	IGUIInterfaceSound::~IGUIInterfaceSound()
	{

	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceSound::Update( real fDeltaTime )
	{
	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceSound::PlayEffect( const CGUIString& rSoundName )
	{
		CGUISoundData* pSound = CGUISoundManager::Instance()->AllocateResource( rSoundName );
		if( !pSound )
		{
			GUI_THROW( GUI_FORMAT( "[IGUIInterfaceSound::PlayMusic]: failed to get music by name <%s>", rSoundName.c_str()));
			return;
		};
		pSound->Load();
		PlayEffect( pSound );
		pSound->RefRelease();
	}
	//------------------------------------------------------------------------------ 
	void IGUIInterfaceSound::PlayMusic( const CGUIString& rMusicName, bool bLoop )
	{
		CGUIMusicData* pMusic = CGUIMusicManager::Instance()->AllocateResource( rMusicName );
		if( !pMusic )
		{
			GUI_THROW( GUI_FORMAT( "[IGUIInterfaceSound::PlayMusic]: failed to get music by name <%s>", rMusicName.c_str()));
			return;
		};
		pMusic->Load();
		PlayMusic( pMusic, bLoop );
		pMusic->RefRelease();
	}
	//------------------------------------------------------------------------------ 
}//namespace guiex


