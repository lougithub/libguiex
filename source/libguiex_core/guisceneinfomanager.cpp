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
#include <libguiex_core/guiexception.h>





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
	int32	CGUISceneInfoManager::LoadScenes( )
	{
		UnloadScenes();
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();

		//get file interface
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		pFileSys->FindFiles( "./", ".uip", m_vecSceneFilePaths );


		//load all scenes
		std::vector<CGUIString> vecErrorList;
		for( uint32 i=0; i<m_vecSceneFilePaths.size(); ++i )
		{
			CGUISceneInfo* pSceneInfo = pConfigFile->LoadSceneInfoFile( m_vecSceneFilePaths[i] );
			if( !pSceneInfo )
			{
				//failed
				vecErrorList.push_back(m_vecSceneFilePaths[i]);
			}
			else
			{
				std::map<CGUIString, CGUISceneInfo*>::iterator itor = m_mapSceneInfos.find( pSceneInfo->GetSceneFilename() );
				if( itor != m_mapSceneInfos.end() )
				{
					DestroySceneInfo( pSceneInfo );
					throw CGUIException(
						"[CGUISceneInfoManager::LoadScenes]: has duplicated scene <%s>", 
						pSceneInfo->GetSceneFilename().c_str());
					return -1;
				}
				m_mapSceneInfos.insert( std::make_pair( pSceneInfo->GetSceneFilename(), pSceneInfo));
				m_vecSceneFileNames.push_back( pSceneInfo->GetSceneFilename() );
			}
		}

		//check error's
		if(!vecErrorList.empty())
		{
			CGUIString errorList;
			for( uint32 i=0; i<vecErrorList.size(); ++i )
			{
				errorList += vecErrorList[i];
				errorList += "\n";
			}
			throw CGUIException(
				"[CGUISceneInfoManager::LoadScenes]: failed to load scene file <%s>", 
				errorList.c_str());
			return -1;
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
	CGUIString	CGUISceneInfoManager::GetSceneFileRootPath( const CGUIString& rSceneName ) const
	{
		CGUISceneInfo * pSceneInfo = GetSceneInfo( rSceneName );
		if( !pSceneInfo )
		{
			return CGUIString();
			//throw CGUIException("[CGUISceneInfoManager::GetSceneFileRootPath]: failed to get scene info by name %s", rSceneName.c_str());
		}
		return pSceneInfo->GetSceneFileRootPath();
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