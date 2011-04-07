/** 
* @file guilocalizationmanager.cpp
* @brief localization manager
* @author ken
* @date 2011-04-01
*/


//============================================================================//
// include
//============================================================================// 
#include "guilocalizationmanager.h"
#include "guiexception.h"
#include "guiinterfacefilesys.h"
#include "guiinterfacemanager.h"
#include "guistringconvertor.h"

#include <tinyxml.h>


//============================================================================//
// function
//============================================================================// 

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUILocalizationManager* CGUILocalizationManager::m_pSingleton = NULL;
	//------------------------------------------------------------------------------
	CGUILocalizationManager::CGUILocalizationManager()
		:m_strCurrentLocalConfig(GUI_DEFAULT_LOCALIZATION)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUILocalizationManager::CGUILocalizationManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUILocalizationManager::~CGUILocalizationManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUILocalizationManager* CGUILocalizationManager::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	const CGUIString& CGUILocalizationManager::GetCurrentLocalConfig() const
	{
		return m_strCurrentLocalConfig;
	}
	//------------------------------------------------------------------------------
	void CGUILocalizationManager::SetCurrentLocalConfig( const CGUIString& rLocalConfig )
	{
		m_strCurrentLocalConfig = rLocalConfig;
	}
	//------------------------------------------------------------------------------
	int32 CGUILocalizationManager::LoadLocalization( const CGUIString& rFileName, const CGUIString& rSceneName)
	{
		///read file
		IGUIInterfaceFileSys* pFileSys =  CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		CGUIDataChunk aDataChunk;
		if( 0 != pFileSys->ReadFile( rFileName, aDataChunk, IGUIInterfaceFileSys::eOpenMode_String ))
		{
			//failed
			throw CGUIException("[CGUILocalizationManager::LoadLocalization]: failed to read file <%s>!", rFileName.c_str());
			return -1;
		}

		///parse file
		TiXmlDocument aDoc;
		aDoc.Parse( (const char*)aDataChunk.GetDataPtr(), 0, TIXML_ENCODING_UTF8 );
		if( aDoc.Error())
		{
			//failed to parse
			throw CGUIException(
				"[CGUILocalizationManager::LoadLocalization]: failed to parse file <%s>!\n\n<%s>", 
				rFileName.c_str(),
				aDoc.ErrorDesc());
			return -1;
		}

		///get root node
		TiXmlElement* pRootNode = aDoc.RootElement();
		if( !pRootNode )
		{
			throw guiex::CGUIException("[CGUILocalizationManager::LoadLocalization], failed to get root node from file <%s>!", rFileName.c_str());
			return -1;
		}


		TMapKeys* pMapKeys  = NULL;
		TMapScenes::iterator itorLoc = m_mapScenes.find( rSceneName );
		if( itorLoc != m_mapScenes.end() )
		{
			pMapKeys = &(itorLoc->second);
		}
		else
		{
			pMapKeys = &(m_mapScenes.insert( std::make_pair(rSceneName, TMapKeys())).first->second);
		}

		//	<localization language="english">
		TiXmlElement* pLocalizationNode = pRootNode->FirstChildElement();
		while( pLocalizationNode )
		{
			const char* szLocal = pLocalizationNode->Attribute("local");
			if( szLocal && m_strCurrentLocalConfig == szLocal )
			{
				//import this local:  <string name="ok"> ok </string>
				TiXmlElement* pStringNode = pLocalizationNode->FirstChildElement();
				while( pStringNode )
				{
					const char* szKey = pStringNode->Attribute("key");
					if( !szKey )
					{
						throw guiex::CGUIException("[CGUILocalizationManager::LoadLocalization], failed to get localization name from local <%s>!", m_strCurrentLocalConfig.c_str());
						return -1;
					}
#if GUI_DEBUG
					if( pMapKeys->find( szKey ) != pMapKeys->end() )
					{
						throw guiex::CGUIException("[CGUILocalizationManager::LoadLocalization], <%s> has existed in localization <%s>!", szKey, m_strCurrentLocalConfig.c_str());
						return -1;
					}
#endif
					const char* szContent = pStringNode->GetText();
					if( !szContent )
					{
						throw guiex::CGUIException("[CGUILocalizationManager::LoadLocalization], failed to get localization content from name from local <%s>!", szKey, m_strCurrentLocalConfig.c_str());
						return -1;
					}
					CGUIStringW strContentW;
					AppMultiByteToWideChar( szContent, strContentW );
					if( strContentW.empty() )
					{
						throw guiex::CGUIException("[CGUILocalizationManager::LoadLocalization], failed to get localization content from name from local <%s>!", szKey, m_strCurrentLocalConfig.c_str());
						return -1;
					}

					pMapKeys->insert( std::make_pair( CGUIString(szKey), strContentW ));

					pStringNode = pStringNode->NextSiblingElement();
				}
			}

			pLocalizationNode = pLocalizationNode->NextSiblingElement();
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUILocalizationManager::UnloadLocalization( const CGUIString& rSceneName)
	{
		TMapScenes::iterator itor = m_mapScenes.find( rSceneName );
		if( itor != m_mapScenes.end() )
		{
			m_mapScenes.erase( itor );
		}
	}
	//------------------------------------------------------------------------------
	CGUIStringW CGUILocalizationManager::Localize(  const CGUIString& rSceneName, const CGUIString& rKey  ) const
	{
		TMapScenes::const_iterator itorScene = m_mapScenes.find( rSceneName );
		if( itorScene != m_mapScenes.end() )
		{
			TMapKeys::const_iterator itorKey = itorScene->second.find( rKey );
			if( itorKey != itorScene->second.end() )
			{
				return itorKey->second;
			}
		}
		return AppMultiByteToWideChar( rKey );
	}
	//------------------------------------------------------------------------------
	CGUIStringW CGUILocalizationManager::Localize( const CGUIString& rFullKey ) const
	{
		///< entry should be "@scenename/key"
		if( rFullKey.empty() )
		{
			return AppMultiByteToWideChar( rFullKey );
		}
		else if( rFullKey[0] != '@' )
		{
			return AppMultiByteToWideChar( rFullKey );
		}

		uint32 index = rFullKey.find_first_of( "/", 1 );
		if( index == uint32(CGUIString::npos ))
		{
			return AppMultiByteToWideChar( rFullKey );
		}

		CGUIString rSceneName = rFullKey.substr( 1, index-1 );
		CGUIString rKey = rFullKey.substr( index+1 );

		TMapScenes::const_iterator itorScene = m_mapScenes.find( rSceneName );
		if( itorScene == m_mapScenes.end() )
		{
			return AppMultiByteToWideChar( rFullKey );
		}
		TMapKeys::const_iterator itorKey = itorScene->second.find( rKey );
		if( itorKey == itorScene->second.end() )
		{
			return AppMultiByteToWideChar( rFullKey );
		}
		return itorKey->second;
	}
	//------------------------------------------------------------------------------
	CGUIStringW CGUILocalizationManager::Localize( const CGUIStringW& rFullKey ) const
	{
		///< entry should be "@scenename/key"
		if( rFullKey.empty() )
		{
			return rFullKey;
		}
		else if( rFullKey[0] != L'@' )
		{
			return rFullKey;
		}

		uint32 index = rFullKey.find_first_of( L"/", 1 );
		if( index == uint32(CGUIStringW::npos ))
		{
			return rFullKey; 
		}

		CGUIString rSceneName = AppWideByteToMultiChar(rFullKey.substr( 1, index-1 ));
		CGUIString rKey = AppWideByteToMultiChar(rFullKey.substr( index+1 ));

		TMapScenes::const_iterator itorScene = m_mapScenes.find( rSceneName );
		if( itorScene == m_mapScenes.end() )
		{
			return rFullKey;
		}
		TMapKeys::const_iterator itorKey = itorScene->second.find( rKey );
		if( itorKey == itorScene->second.end() )
		{
			return rFullKey;
		}
		return itorKey->second;
	}
	//------------------------------------------------------------------------------
	void CGUILocalizationManager::GetAllLocalizations( std::vector<CGUIString>& rArrays ) const
	{
		for( TMapScenes::const_iterator itorScene = m_mapScenes.begin(); itorScene != m_mapScenes.end(); ++itorScene )
		{
			for( TMapKeys::const_iterator itorKey = itorScene->second.begin(); itorKey != itorScene->second.end(); ++itorKey )
			{
				rArrays.push_back( CGUIString("@") + itorScene->first + "/" + itorKey->first );
			}
		}
	}
	//------------------------------------------------------------------------------
}//namespace guiex


