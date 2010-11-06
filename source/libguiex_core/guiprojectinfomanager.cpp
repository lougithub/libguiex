/** 
 * @file guiprojectinfomanager.cpp
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-22
 */






//============================================================================//
// include
//============================================================================//
#include <libguiex_core\guiprojectinfomanager.h>
#include <libguiex_core\guiprojectinfo.h>
#include <libguiex_core\guiwidgetsystem.h>
#include <libguiex_core\guiinterfacefilesys.h>
#include <libguiex_core\guiinterfacemanager.h>
#include <libguiex_core\guiinterfaceconfigfile.h>
#include <libguiex_core\guiexception.h>





//============================================================================//
// function
//============================================================================//

namespace guiex
{
	//------------------------------------------------------------------------------
	GUI_SINGLETON_IMPLEMENT_EX(CGUIProjectInfoManager );
	//------------------------------------------------------------------------------
	CGUIProjectInfoManager::CGUIProjectInfoManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUIProjectInfoManager::~CGUIProjectInfoManager()
	{
		UnloadProjects();
	}
	//------------------------------------------------------------------------------
	int32	CGUIProjectInfoManager::LoadProjects( )
	{
		UnloadProjects();
		IGUIInterfaceConfigFile* pConfigFile = CGUIInterfaceManager::Instance()->GetInterfaceConfigFile();

		//get file interface
		IGUIInterfaceFileSys* pFileSys = CGUIInterfaceManager::Instance()->GetInterfaceFileSys();
		pFileSys->FindFiles( ".\\", "*.uip", m_vecProjectFilePaths );


		//load all projects
		std::vector<CGUIString> vecErrorList;
		for( uint32 i=0; i<m_vecProjectFilePaths.size(); ++i )
		{
			CGUIProjectInfo* pProjectInfo = pConfigFile->LoadProjectInfoFile( m_vecProjectFilePaths[i] );
			if( !pProjectInfo )
			{
				//failed
				vecErrorList.push_back(m_vecProjectFilePaths[i]);
			}
			else
			{
				std::map<CGUIString, CGUIProjectInfo*>::iterator itor = m_mapProjectInfos.find( pProjectInfo->GetProjectFilename() );
				if( itor != m_mapProjectInfos.end() )
				{
					DestroyProjectInfo( pProjectInfo );
					throw CGUIException(
						"[CGUIProjectInfoManager::LoadProjects]: has duplicated project <%s>", 
						pProjectInfo->GetProjectFilename().c_str());
					return -1;
				}
				m_mapProjectInfos.insert( std::make_pair( pProjectInfo->GetProjectFilename(), pProjectInfo));
				m_vecProjectFileNames.push_back( pProjectInfo->GetProjectFilename() );
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
				"[CGUIProjectInfoManager::LoadProjects]: failed to load project file <%s>", 
				errorList.c_str());
			return -1;
		}

		//check dependencies
		for( std::map<CGUIString, CGUIProjectInfo*>::iterator itor = m_mapProjectInfos.begin();
			itor != m_mapProjectInfos.end(); 
			++itor )
		{
			CGUIProjectInfo* pInfo = itor->second;
			pInfo->SetDependenciesLoaded(true);
			const std::vector<CGUIString>&	vecDependencies = pInfo->GetDependencies();
			for( uint32 i=0; i<vecDependencies.size(); ++i )
			{
				if( !GetProjectInfo(vecDependencies[i]))
				{
					pInfo->SetDependenciesLoaded(false);
					break;
				}
			}
		}
		return 0;
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfoManager::UnloadProjects( )
	{
		for( std::map<CGUIString, CGUIProjectInfo*>::iterator itor = m_mapProjectInfos.begin();
			itor != m_mapProjectInfos.end();
			++itor)
		{
			DestroyProjectInfo( itor->second );
		}	
		m_mapProjectInfos.clear();

		m_vecProjectFilePaths.clear();
		m_vecProjectFileNames.clear();
	}
	//------------------------------------------------------------------------------
	CGUIProjectInfo*	CGUIProjectInfoManager::GetProjectInfo( const CGUIString& rProjectName ) const
	{
		std::map<CGUIString, CGUIProjectInfo*>::const_iterator itor = m_mapProjectInfos.find( rProjectName );
		if( itor != m_mapProjectInfos.end())
		{
			return itor->second;
		}
		else
		{
			return NULL;
		}
	}
	//------------------------------------------------------------------------------
	void	CGUIProjectInfoManager::ClearProjectLoadFlags( )
	{
		for( std::map<CGUIString, CGUIProjectInfo*>::iterator itor = m_mapProjectInfos.begin();
			itor != m_mapProjectInfos.end();
			++itor )
		{
			itor->second->SetResourceLoaded(false );
		}
	}
	//------------------------------------------------------------------------------
	const CGUIString&	CGUIProjectInfoManager::GetProjectFilePath( const CGUIString& rProjectName ) const
	{
		CGUIProjectInfo * pProjectInfo = GetProjectInfo( rProjectName );
		if( !pProjectInfo )
		{
			throw CGUIException("[CGUIProjectInfoManager::GetProjectFilePath]: failed to get project info by name %s", rProjectName.c_str());
		}
		return pProjectInfo->GetProjectFilePath();
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfoManager::GetProjectFilePaths( ) const
	{
		return m_vecProjectFilePaths;
	}
	//------------------------------------------------------------------------------
	const std::vector<CGUIString>&	CGUIProjectInfoManager::GetProjectFileNames( ) const
	{
		return m_vecProjectFileNames;
	}
	//------------------------------------------------------------------------------
	CGUIProjectInfo* CGUIProjectInfoManager::GenerateProjectInfo() const
	{
		return new CGUIProjectInfo;
	}
	//------------------------------------------------------------------------------
	void CGUIProjectInfoManager::DestroyProjectInfo( CGUIProjectInfo* pProjectInfo) const
	{
		delete pProjectInfo;
	}
	//------------------------------------------------------------------------------
}