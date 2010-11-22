/** 
 * @file guisceneinfomanager.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-22
 */






//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisceneinfomanager.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiinterfacefilesys.h>
#include <libguiex_core/guiinterfacemanager.h>
#include <libguiex_core/guiinterfaceconfigfile.h>
#include <libguiex_core/guiproperty.h>
#include <libguiex_core/guiexception.h>


#if defined( GUIEX_PLATFORM_MAC )
#include <libgen.h>
#endif


//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_SINGLETON_IMPLEMENT_EX(CGUISceneInfoManager );
	//------------------------------------------------------------------------------
	CGUISceneInfoManager::CGUISceneInfoManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUISceneInfoManager::~CGUISceneInfoManager()
	{
		UnloadScenes();
	}
	//------------------------------------------------------------------------------
	CGUIString CGUISceneInfoManager::DoGetFilename( const CGUIString& rPath ) 
	{
#if defined( GUIEX_PLATFORM_WIN32 )
		char fname[_MAX_FNAME];
		char fext[_MAX_EXT];
		_splitpath( rPath.c_str(), NULL, NULL, fname, fext ); 
		return CGUIString( fname ) + fext;
#elif defined( GUIEX_PLATFORM_MAC )
		char* pBuf = new char[rPath.size()+1];
		strcpy( pBuf, rPath.c_str() );
		CGUIString aBaseName(basename( pBuf));
		delete[] pBuf;
		return aBaseName;
#else
#	error "unknown platform"		
#endif
	}
	//------------------------------------------------------------------------------
	CGUIString CGUISceneInfoManager::DoGetFileDir( const CGUIString& rPath ) 
	{
#if defined( GUIEX_PLATFORM_WIN32 )
		char fdir[_MAX_DIR];
		_splitpath( rPath.c_str(), NULL, fdir, NULL, NULL ); 
		return CGUIString( fdir );
#elif defined( GUIEX_PLATFORM_MAC )
		char* pBuf = new char[rPath.size()+1];
		strcpy( pBuf, rPath.c_str() );
		CGUIString aDirName(dirname( pBuf));
		aDirName += "/";
		delete[] pBuf;
		return aDirName;
#else
#	error "unknown platform"		
#endif
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneInfoManager::DoLoadScene( const CGUIString& rSceneFilePath )
	{
		//get interface of config file
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();
		if( !pConfigFile )
		{
			throw CGUIException( "[CGUISceneInfoManager::DoLoadScene]: failed to get config file interface." );
			return -1;
		}

		//get property set
		CGUIProperty aPropertySet;
		if( 0 != pConfigFile->LoadConfigFile( rSceneFilePath, aPropertySet ))
		{
			throw CGUIException( 
				"[CGUISceneInfoManager::DoLoadScene]: failed to read scene info config file <%s>.",
				rSceneFilePath.c_str() );
			return -1;
		}

		//parse scene info
		CGUISceneInfo * pSceneInfo = GenerateSceneInfo();
		if( 0 != pSceneInfo->LoadFromPropertySet( DoGetFilename(rSceneFilePath), DoGetFileDir(rSceneFilePath), aPropertySet ))
		{
			DestroySceneInfo( pSceneInfo );
			return -1;
		}

		std::map<CGUIString, CGUISceneInfo*>::iterator itor = m_mapSceneInfos.find( pSceneInfo->GetSceneFilename() );
		if( itor != m_mapSceneInfos.end() )
		{
			DestroySceneInfo( pSceneInfo );
			throw CGUIException(
				"[CGUISceneInfoManager::DoLoadScene]: has duplicated scene <%s>", 
				pSceneInfo->GetSceneFilename().c_str());
			return -1;
		}
		m_mapSceneInfos.insert( std::make_pair( pSceneInfo->GetSceneFilename(), pSceneInfo));
		m_vecSceneFileNames.push_back( pSceneInfo->GetSceneFilename() );

		return 0;
	}
	//------------------------------------------------------------------------------
	/*
	* @param rSceneRootPath root path of all scenes.
	* @param rSuffix suffix of project file.
	*/
	int32 CGUISceneInfoManager::LoadScenes( const CGUIString& rSceneRootPath, const CGUIString& rSuffix )
	{
		UnloadScenes();

		//get file interface
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		pFileSys->FindFiles( rSceneRootPath, rSuffix, m_vecSceneFilePaths );

		//load all scenes
		std::vector<CGUIString> vecErrorList;
		for( uint32 i=0; i<m_vecSceneFilePaths.size(); ++i )
		{
			if( 0 != DoLoadScene( m_vecSceneFilePaths[i] ))
			{
				return -1;
			}
		}

		//check dependencies
		for( std::map<CGUIString, CGUISceneInfo*>::iterator itor = m_mapSceneInfos.begin();
			itor != m_mapSceneInfos.end(); 
			++itor )
		{
			CGUISceneInfo* pInfo = itor->second;
			pInfo->SetDependenciesLoaded(true);
			const std::vector<CGUIString>&	vecDependencies = pInfo->GetDependencies();
			for( uint32 i=0; i<vecDependencies.size(); ++i )
			{
				if( !GetSceneInfo(vecDependencies[i]))
				{
					pInfo->SetDependenciesLoaded(false);
					break;
				}
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUISceneInfoManager::UnloadScenes( )
	{
		for( std::map<CGUIString, CGUISceneInfo*>::iterator itor = m_mapSceneInfos.begin();
			itor != m_mapSceneInfos.end();
			++itor)
		{
			DestroySceneInfo( itor->second );
		}	
		m_mapSceneInfos.clear();

		m_vecSceneFilePaths.clear();
		m_vecSceneFileNames.clear();
	}
	//------------------------------------------------------------------------------
	/**
	* @brief 
	* @param rSceneFile the filename of scenes, for example <test.uip>
	*/
	CGUISceneInfo*	CGUISceneInfoManager::GetSceneInfo( const CGUIString& rSceneName ) const
	{
		std::map<CGUIString, CGUISceneInfo*>::const_iterator itor = m_mapSceneInfos.find( rSceneName );
		if( itor != m_mapSceneInfos.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUISceneInfoManager::ClearSceneResourceLoadFlags( )
	{
		for( std::map<CGUIString, CGUISceneInfo*>::iterator itor = m_mapSceneInfos.begin();
			itor != m_mapSceneInfos.end();
			++itor )
		{
			itor->second->SetResourceLoaded(false );
		}
	}
	//------------------------------------------------------------------------------
	CGUIString	CGUISceneInfoManager::GetScenePath( const CGUIString& rSceneName ) const
	{
		CGUISceneInfo * pSceneInfo = GetSceneInfo( rSceneName );
		if( !pSceneInfo )
		{
			return CGUIString();
			//throw CGUIException("[CGUISceneInfoManager::GetScenePath]: failed to get scene info by name %s", rSceneName.c_str());
		}
		return pSceneInfo->GetScenePath();
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUISceneInfoManager::GetSceneFilePaths( ) const
	{
		return m_vecSceneFilePaths;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUISceneInfoManager::GetSceneFileNames( ) const
	{
		return m_vecSceneFileNames;
	}
	//------------------------------------------------------------------------------
	CGUISceneInfo* CGUISceneInfoManager::GenerateSceneInfo() const
	{
		return new CGUISceneInfo;
	}
	//------------------------------------------------------------------------------
	void CGUISceneInfoManager::DestroySceneInfo( CGUISceneInfo* pSceneInfo) const
	{
		delete pSceneInfo;
	}
	//------------------------------------------------------------------------------
}