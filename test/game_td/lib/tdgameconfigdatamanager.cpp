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
		m_mapProperty.clear();

		//get file list
		std::vector<CGUIString> vecSceneFilePaths;
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		pFileSys->FindFiles( rPath, ".xml", vecSceneFilePaths );

		//load property
		for( uint32 i=0; i<vecSceneFilePaths.size(); ++i )
		{
			//get file name
			CGUIString strFilename;
			CGUIString::size_type nPos = vecSceneFilePaths[i].rfind( "/" );
			if( nPos == CGUIString::npos )
			{
				nPos = vecSceneFilePaths[i].rfind( "\\" );
			}
			if( nPos == CGUIString::npos )
			{
				strFilename = vecSceneFilePaths[i];
			}
			else
			{
				strFilename = vecSceneFilePaths[i].substr( nPos+1 );
			}

			//get property set
			m_mapProperty[strFilename] = CGUIProperty();
			CGUIProperty& rProperty = m_mapProperty[strFilename];

			IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
			if( 0 != pConfigFile->LoadConfigFile(vecSceneFilePaths[i], rProperty) )
			{
				GUI_THROW( GUI_FORMAT("[CTDGameConfigDataManager::LoadData]: failed to load config file <%s>!", vecSceneFilePaths[i].c_str() ));
				return;
			}
		}
	}
	//------------------------------------------------------------------------------
	void CTDGameConfigDataManager::ClearData(  )
	{
		m_mapProperty.clear();
	}
	//------------------------------------------------------------------------------
	const CGUIProperty* CTDGameConfigDataManager::GetProperty( const CGUIString& rFilename ) const
	{
		TMapProperty::const_iterator itor = m_mapProperty.find( rFilename );
		if( itor == m_mapProperty.end())
		{
			return NULL;
		}
		else
		{
			return &itor->second;
		}
	}
	//------------------------------------------------------------------------------
}

