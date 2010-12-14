/** 
 * @file guiscenemanager.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-22
 */

//============================================================================//
// include
//============================================================================//
#include "guiscenemanager.h"
#include "guiscene.h"
#include "guisystem.h"
#include "guiinterfacefilesys.h"
#include "guiinterfacemanager.h"
#include "guiinterfaceconfigfile.h"
#include "guiproperty.h"
#include "guiexception.h"
#include "guiconfigfileloader.h"
#include <algorithm>


#if defined( GUIEX_PLATFORM_MAC )
#include <libgen.h>
#endif


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISceneManager* CGUISceneManager::m_pSingleton = NULL;
	//------------------------------------------------------------------------------
	CGUISceneManager::CGUISceneManager()
	{
		GUI_ASSERT( !m_pSingleton, "[CGUISceneManager::CGUISceneManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUISceneManager::~CGUISceneManager()
	{
		UnregisterAllScenes();

		m_pSingleton = NULL; 
	}
	//------------------------------------------------------------------------------
	CGUISceneManager* CGUISceneManager::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::RegisterScene( const CGUIString& rSceneFilePath, bool bAutoBuildDepency )
	{
		//get interface of config file
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			throw CGUIException( "[CGUISceneManager::RegisterScene]: failed to get config file interface." );
			return -1;
		}

		//get property set
		CGUIProperty aPropertySet;
		if( 0 != pConfigFile->LoadConfigFile( rSceneFilePath, aPropertySet ))
		{
			throw CGUIException( 
				"[CGUISceneManager::RegisterScene]: failed to read scene info config file <%s>.",
				rSceneFilePath.c_str() );
			return -1;
		}
		
		//get interface of file system
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		if( !pFileSys )
		{
			throw CGUIException( "[CGUISceneManager::RegisterScene]: failed to get file sys interface." );
			return -1;
		}

		//parse scene info
		CGUIScene * pScene = GenerateScene( pFileSys->GetFilename(rSceneFilePath) );
		if( 0 != pScene->LoadFromPropertySet( pFileSys->GetFileDir(rSceneFilePath), aPropertySet ))
		{
			DestroyScene( pScene );
			return -1;
		}

		std::map<CGUIString, CGUIScene*>::iterator itor = m_mapScenes.find( pScene->GetSceneName() );
		if( itor != m_mapScenes.end() )
		{
			DestroyScene( pScene );
			throw CGUIException(
				"[CGUISceneManager::RegisterScene]: has duplicated scene <%s>", 
				pScene->GetSceneName().c_str());
			return -1;
		}
		m_mapScenes.insert( std::make_pair( pScene->GetSceneName(), pScene));
		m_vecSceneNames.push_back( pScene->GetSceneName() );

		return 0;
	}
	//------------------------------------------------------------------------------
	/*
	* @brief return 0 if all dependency is done, or number of unloaded dependencies.
	*/
	int32 CGUISceneManager::BuildScenesDependency()
	{
		int32 bSceneNotLoaded = 0;

		//check dependencies
		for( std::map<CGUIString, CGUIScene*>::iterator itor = m_mapScenes.begin();
			itor != m_mapScenes.end(); 
			++itor )
		{
			CGUIScene* pScene = itor->second;
			pScene->SetDependenciesLoaded(true);
			const std::vector<CGUIString>&	vecDependencies = pScene->GetDependencies();
			for( uint32 i=0; i<vecDependencies.size(); ++i )
			{
				if( !GetScene(vecDependencies[i]))
				{
					pScene->SetDependenciesLoaded(false);
					++bSceneNotLoaded;
					break;
				}
			}
		}

		return bSceneNotLoaded;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::CheckScenesDependency( const CGUIString& rSceneName )
	{
#if GUI_DEBUG
		//get scene from scene name
		CGUIScene* pScene = GetScene( rSceneName );
		if( !pScene )
		{
			return -1;
		}

		//check dependencies
		if( pScene->IsDependenciesLoaded() == false )
		{
			throw CGUIException( "[CGUISceneManager::CheckScenesDependenc] scene <%s>'s dependencies is not loaded" );
			return -1;
		}

		const std::vector<CGUIString>& rDependencies = pScene->GetDependencies();
		for( std::vector<CGUIString>::const_iterator itor = rDependencies.begin();
			itor != rDependencies.end();
			++itor )
		{
			if( !CheckScenesDependency( *itor ) )
			{
				return -1;
			}
		}
#endif //GUI_DEBUG
		return 0;
	}
	//------------------------------------------------------------------------------
	/*
	* @param rSceneRootPath root path of all scenes.
	* @param rSuffix suffix of project file.
	*/
	int32 CGUISceneManager::RegisterScenesFromDir( const CGUIString& rSceneRootPath, const CGUIString& rSuffix, bool bAutoBuildDepency /*= true*/ )
	{
		UnregisterAllScenes();

		//get file interface
		std::vector<CGUIString> vecSceneFilePaths; //absolute file paths
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		pFileSys->FindFiles( rSceneRootPath, rSuffix, vecSceneFilePaths );

		//load all scenes
		std::vector<CGUIString> vecErrorList;
		for( uint32 i=0; i<vecSceneFilePaths.size(); ++i )
		{
			if( 0 != RegisterScene( vecSceneFilePaths[i], false ))
			{
				return -1;
			}
		}
	
		if( bAutoBuildDepency )
		{
			BuildScenesDependency();
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::UnregisterAllScenes( )
	{
		for( std::map<CGUIString, CGUIScene*>::iterator itor = m_mapScenes.begin();
			itor != m_mapScenes.end();
			++itor)
		{
			CGUIScene* pScene = itor->second;
			if( pScene->GetRefCount() != 0 )
			{
				throw CGUIException(
					"[CGUISceneManager::UnregisterAllScenes]: the scene <%s> which will be unregistered still has some reference.",
					pScene->GetSceneName().c_str());
				return;
			}
			DestroyScene( pScene );
		}	
		m_mapScenes.clear();
		m_vecSceneNames.clear();
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::UnregisterScene( const CGUIString& rSceneName )
	{
		//find scene by name
		std::map<CGUIString, CGUIScene*>::iterator itorScene = m_mapScenes.find( rSceneName );
		if( itorScene == m_mapScenes.end() )
		{
			throw CGUIException(
				"[CGUISceneManager::UnregisterScene]: failed to unregister scene <%s> for not finding it.",
				rSceneName.c_str());
			return;
		}

		//check reference
		CGUIScene* pScene = itorScene->second;
		if( pScene->GetRefCount() != 0 )
		{
			throw CGUIException(
				"[CGUISceneManager::UnregisterScene]: the scene <%s> which will be unregistered still has some reference.",
				pScene->GetSceneName().c_str());
			return;
		}
		DestroyScene( pScene );
		m_mapScenes.erase( itorScene );

		//remove name from list
		std::vector<CGUIString>::iterator itorName = std::find( m_vecSceneNames.begin(), m_vecSceneNames.end(), rSceneName );
		if( itorName != m_vecSceneNames.end() )
		{
			m_vecSceneNames.erase( itorName );
		}
		else
		{
			throw CGUIException(
				"[CGUISceneManager::UnregisterScene]: can't find scene <%s> in scene list.",
				pScene->GetSceneName().c_str());
		}
	}
	//------------------------------------------------------------------------------
	CGUIScene* CGUISceneManager::GetScene( const CGUIString& rSceneName ) const
	{
		std::map<CGUIString, CGUIScene*>::const_iterator itor = m_mapScenes.find( rSceneName );
		if( itor != m_mapScenes.end())
		{
			return itor->second;
		}
		else
		{
			throw CGUIException( "[CGUISceneManager::GetScene] failed to get scene by name <%s>", rSceneName.c_str() );
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::ClearSceneResourceLoadFlags( )
	{
		for( std::map<CGUIString, CGUIScene*>::iterator itor = m_mapScenes.begin();
			itor != m_mapScenes.end();
			++itor )
		{
			itor->second->SetResourceLoaded(false );
		}
	}
	//------------------------------------------------------------------------------
	CGUIString CGUISceneManager::GetScenePath( const CGUIString& rSceneName ) const
	{
		CGUIScene * pScene = GetScene( rSceneName );
		if( !pScene )
		{
			throw CGUIException("[CGUISceneManager::GetScenePath]: failed to get scene info by name %s", rSceneName.c_str());
		}
		return pScene->GetScenePath();
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUISceneManager::GetSceneNames( ) const
	{
		return m_vecSceneNames;
	}
	//------------------------------------------------------------------------------
	CGUIScene* CGUISceneManager::GenerateScene( const CGUIString& rSceneName ) const
	{
		return new CGUIScene( rSceneName );
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::DestroyScene( CGUIScene* pScene) const
	{
		delete pScene;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::LoadResourceImp(const CGUIString& strSceneName)
	{
		//get scene
		CGUIScene* pSceneInfo = GetScene( strSceneName );
		if( !pSceneInfo )
		{
			throw CGUIException( "[CGUISceneUtility::LoadResourceImp] failed to load resource by scene name <%s>", strSceneName.c_str());
			return -1;
		}

		//load dependencies first
		const std::vector<CGUIString>& rDependencies = pSceneInfo->GetDependencies();
		for( std::vector<CGUIString>::const_iterator itor = rDependencies.begin();
			itor != rDependencies.end();
			++itor )
		{
			if( 0 != LoadResourceImp( *itor ) )
			{
				return -1;
			}
		}

		//load this scene
		if( pSceneInfo->GetRefCount() == 0 )
		{
			const std::vector<CGUIString>& rResourceFiles = pSceneInfo->GetResourceFiles( );
			for( uint32 i=0; i<rResourceFiles.size(); ++i )
			{
				CGUIString strResourceFilePath = pSceneInfo->GetScenePath() + rResourceFiles[i];	
				if( 0 != CGUIConfigFileLoader::LoadResourceConfigFile(strResourceFilePath, strSceneName, rResourceFiles[i]))
				{
					throw CGUIException( "[CGUISceneUtility::LoadResourceImp] failed to load resource by scene name <%s : %s>",strResourceFilePath.c_str(), strSceneName.c_str());
					return -1;
				}
			}
		}
		pSceneInfo->RefRetain();

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::UnloadResourceImp(const CGUIString& strSceneName)
	{
		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::LoadResource(const CGUIString& strSceneName)
	{
		//check dependency
		if( 0 != CheckScenesDependency( strSceneName ) )
		{
			return -1;
		}

		return LoadResourceImp( strSceneName );
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::UnloadResource(const CGUIString& strSceneName)
	{
		//check dependency
		if( 0 != CheckScenesDependency( strSceneName ) )
		{
			return -1;
		}

		return UnloadResourceImp( strSceneName );
	}
	//------------------------------------------------------------------------------
}