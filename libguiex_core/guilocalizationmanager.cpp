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
#include "guiinterfacelocalizationloader.h"
#include "guiinterfacemanager.h"
#include "guistringconvertor.h"
#include "guilogmsgmanager.h"

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
		GUI_TRACE( GUI_FORMAT("[CGUILocalizationManager::LoadLocalization] name:%s, scene:%s", rFileName.c_str(), rSceneName.c_str()) );
		///read file
		IGUIInterfaceLocalizationLoader* pLoader = CGUIInterfaceManager::Instance()->GetInterfaceLocalizationLoader();

		std::vector<std::pair<CGUIString, CGUIStringW> > arrayLocalizations;
		if( 0 != pLoader->LoadLocalizationFile(rFileName, m_strCurrentLocalConfig, arrayLocalizations ))
		{
			//failed to parse
			CGUIException::ThrowException(
				"[CGUILocalizationManager::LoadLocalization]: failed to parse file <%s>!\n", 
				rFileName.c_str());
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


		for( std::vector<std::pair<CGUIString, CGUIStringW> >::iterator itor = arrayLocalizations.begin();
			itor != arrayLocalizations.end();
			++itor )
		{
			std::pair<CGUIString, CGUIStringW>& rLocal = *itor;
#if GUI_DEBUG
			if( pMapKeys->find( rLocal.first ) != pMapKeys->end() )
			{
				CGUIException::ThrowException("[CGUILocalizationManager::LoadLocalization], <%s> has existed in localization <%s>!", rLocal.first.c_str(), m_strCurrentLocalConfig.c_str());
				return -1;
			}
#endif
			pMapKeys->insert( rLocal );
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


