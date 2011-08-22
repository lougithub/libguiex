/** 
* @file tdgameworldbase.cpp
* @brief base class of game world, contain all monster, bullet and thing in fighting game
* @author ken
* @date 2011-08-19
*/


//============================================================================//
// include
//============================================================================// 
#include "tdgameconfigdatamanager.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	CTDGameConfigDataManager::CTDGameConfigDataManager()
	{

	}
	//------------------------------------------------------------------------------
	CTDGameConfigDataManager::~CTDGameConfigDataManager()
	{
	}
	//------------------------------------------------------------------------------
	void CTDGameConfigDataManager::LoadData( const CGUIString& rPath )
	{
		m_aMapProperty.clear();

		//get file list
		std::vector<CGUIString> vecSceneFilePaths;
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		pFileSys->FindFiles( rPath, ".xml", vecSceneFilePaths );

		//load property
		for( uint32 i=0; i<vecSceneFilePaths.size(); ++i )
		{
			//get property set
			m_aMapProperty[vecSceneFilePaths[i]] = CGUIProperty();
			CGUIProperty& rProperty = m_aMapProperty[vecSceneFilePaths[i]];

			IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
			if( 0 != pConfigFile->LoadConfigFile(vecSceneFilePaths[i], rProperty) )
			{
				GUI_THROW( GUI_FORMAT("[CTDGameConfigDataManager::LoadData]: failed to load config file <%s>!", vecSceneFilePaths[i].c_str() ));
				return;
			}
		}
	}
	//------------------------------------------------------------------------------
}

