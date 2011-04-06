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
#include "guiproperty.h"
#include "guiexception.h"
#include "guiconfigfileloader.h"
#include "guiwidgetmanager.h"
#include "guilocalizationmanager.h"
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
		//get interface of file system
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		if( !pFileSys )
		{
			throw CGUIException( "[CGUISceneManager::RegisterScene]: failed to get file sys interface." );
			return -1;
		}

		//parse scene info
		CGUIScene * pScene = GenerateScene( rSceneFilePath );
		if( 0 != pScene->LoadConfigFile())
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
			if( 0 != CheckScenesDependency( *itor ) )
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
	int32 CGUISceneManager::LoadWidgets( const CGUIString& rSceneName )
	{
		CGUIScene* pScene = GetScene( rSceneName );
		if( !pScene )
		{
			throw CGUIException( "[CGUISceneManager::LoadWidgets] failed to get scene <%s>", rSceneName.c_str());
			return -1;
		}

		const std::vector<CGUIString>& rWidgetFile = pScene->GetWidgetFiles();
		for( uint32 i=0; i<rWidgetFile.size(); ++i )
		{
			CGUIWidget* pPageWidget = CGUIWidgetManager::Instance()->LoadPage( rWidgetFile[i], rSceneName );
			if( !pPageWidget )
			{
				throw CGUIException(
					"[CGUISceneManager::LoadWidgets] failed to create page <%s> in scene <%s>", 
					rWidgetFile[i].c_str(), rSceneName.c_str());
				return -1;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::ReleaseWidgets( const CGUIString& rSceneName )
	{
		CGUIScene* pScene = GetScene( rSceneName );
		if( !pScene )
		{
			throw CGUIException( "[CGUISceneManager::ReleaseWidgets] failed to get scene <%s>", rSceneName.c_str());
			return;
		}

		const std::vector<CGUIString>& rWidgetFile = pScene->GetWidgetFiles();
		for( uint32 i=0; i<rWidgetFile.size(); ++i )
		{
			CGUIWidgetManager::Instance()->ReleasePage( rSceneName, rWidgetFile[i] );
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneManager::LoadResources(const CGUIString& strSceneName)
	{
		//check dependency
		if( 0 != CheckScenesDependency( strSceneName ) )
		{
			return -1;
		}

		//get scene
		CGUIScene* pScene = GetScene( strSceneName );
		if( !pScene )
		{
			throw CGUIException(
				"[CGUISceneManager::LoadResources] failed to load resource by scene name <%s>", 
				strSceneName.c_str());
			return -1;
		}

		//load dependencies first
		const std::vector<CGUIString>& rDependencies = pScene->GetDependencies();
		for( std::vector<CGUIString>::const_iterator itor = rDependencies.begin();
			itor != rDependencies.end();
			++itor )
		{
			if( 0 != LoadResources( *itor ) )
			{
				return -1;
			}
		}

		//load this scene
		if( pScene->GetRefCount() == 0 )
		{
			LoadResourceImp( pScene );
		}
		pScene->RefRetain();

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::ReleaseResources(const CGUIString& strSceneName)
	{
		//check dependency
		if( 0 != CheckScenesDependency( strSceneName ) )
		{
			return;
		}

		//get scene
		CGUIScene* pScene = GetScene( strSceneName );
		if( !pScene )
		{
			throw CGUIException( 
				"[CGUISceneManager::ReleaseResources] failed to load resource by scene name <%s>", 
				strSceneName.c_str());
			return;
		}

		//release this scene
		pScene->RefRelease();
		if( pScene->GetRefCount() == 0 )
		{
			ReleaseResourceImp( pScene );
		}

		//release dependencies first
		const std::vector<CGUIString>& rDependencies = pScene->GetDependencies();
		for( std::vector<CGUIString>::const_iterator itor = rDependencies.begin();
			itor != rDependencies.end();
			++itor )
		{
			ReleaseResources( *itor );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::LoadResourceImp( CGUIScene* pScene )
	{
		//load resource
		const std::vector<CGUIString>& rResourceFiles = pScene->GetResourceFiles( );
		for( uint32 i=0; i<rResourceFiles.size(); ++i )
		{
			CGUIString strResourceFilePath = pScene->GetScenePath() + rResourceFiles[i];	
			if( 0 != CGUIConfigFileLoader::LoadResourceConfigFile(strResourceFilePath, pScene->GetSceneName()))
			{
				throw CGUIException( 
					"[CGUISceneManager::LoadResourceImp] failed to load resource by scene name <%s : %s>",
					strResourceFilePath.c_str(), pScene->GetSceneName().c_str());
			}
		}

		//load localization
		const std::vector<CGUIString>& rLocalizationFiles = pScene->GetLocalizationFiles( );
		for( uint32 i=0; i<rLocalizationFiles.size(); ++i )
		{
			CGUIString strLocalizationFilePath = pScene->GetScenePath() + rLocalizationFiles[i];	
			if( 0 != CGUILocalizationManager::Instance()->LoadLocalization(strLocalizationFilePath, pScene->GetSceneName()))
			{
				throw CGUIException( 
					"[CGUISceneManager::LoadResourceImp] failed to load localization by scene name <%s : %s>",
					strLocalizationFilePath.c_str(), pScene->GetSceneName().c_str());
			}
		}

		//load script
		const std::vector<CGUIString>& rScriptFiles = pScene->GetScriptFiles( );
		for( uint32 i=0; i<rScriptFiles.size(); ++i )
		{
			guiex::IGUIInterfaceScript* pInterfaceScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
			if( pInterfaceScript && !GSystem->IsEditorMode())
			{
				// create script
				pInterfaceScript->CreateScript( pScene->GetSceneName() );

				// load script
				CGUIString strScriptFilePath = pScene->GetScenePath() + rScriptFiles[i];	
				pInterfaceScript->ExecuteFile(strScriptFilePath, pScene->GetSceneName());
			}
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::ReleaseResourceImp( CGUIScene* pScene )
	{
		//release resource
		GSystem->ReleaseResourceByScene( pScene->GetSceneName() );

		//unload localization
		CGUILocalizationManager::Instance()->UnloadLocalization( pScene->GetSceneName() );

		//release script
		guiex::IGUIInterfaceScript* pInterfaceScript = CGUIInterfaceManager::Instance()->GetInterfaceScript();
		if( pInterfaceScript )
		{
			// destroy script
			pInterfaceScript->DestroyScript( pScene->GetSceneName() );
		}
	}
	//------------------------------------------------------------------------------
	void CGUISceneManager::ReleaseAllResources( )
	{
		for( std::map<CGUIString, CGUIScene*>::iterator itor = m_mapScenes.begin();
			itor != m_mapScenes.end();
			++itor )
		{
			CGUIScene* pScene = itor->second;
			if( pScene->GetRefCount() != 0 )
			{
				ReleaseResourceImp( pScene );
				pScene->RefClear();
			}
		}
	}
	//------------------------------------------------------------------------------
}