/** 
* @file guiprojectutility.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/






//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guiprojectutility.h>
#include <libguiex_core/guiprojectinfo.h>
#include <libguiex_core/guiprojectinfomanager.h>
#include <libguiex_core/guiwidgetsystem.h>
#include <libguiex_core/guiexception.h>




//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	int32 CGUIProjectUtility::LoadResource(const CGUIString& strProjectName)
	{
		CGUIProjectInfo* pProjectInfo = CGUIProjectInfoManager::Instance()->GetProjectInfo( strProjectName );
		if( !pProjectInfo )
		{
			throw CGUIException( "[CGUIProjectUtility::LoadResource] failed to load resource by project name <%s>", strProjectName.c_str());
			return -1;
		}

		if( pProjectInfo->IsResourceLoaded())
		{
			//has been loaded
			return 0;
		}
			//load this project
		const std::vector<CGUIString>& rResourceFiles = pProjectInfo->GetResourceFiles( );
		for( uint32 i=0; i<rResourceFiles.size(); ++i )
		{
			CGUIString strResourceFilePath = pProjectInfo->GetProjectFilePath() + rResourceFiles[i];	
			if( 0 != CGUIWidgetSystem::Instance()->LoadResource(strResourceFilePath, strProjectName))
			{
				throw CGUIException( "[CGUIProjectUtility::LoadResource] failed to load resource by project name <%s : %s>",strResourceFilePath.c_str(), strProjectName.c_str());
				return -1;
			}
		}
		pProjectInfo->SetResourceLoaded(true);	

		//load dependencies
		const std::vector<CGUIString>&rDependencies = pProjectInfo->GetDependencies();
		for( uint32 i=0; i<rDependencies.size(); ++i )
		{
			if( 0 != LoadResource(rDependencies[i]))
			{
				return -1;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
	int32 CGUIProjectUtility::UnloadResource(const CGUIString& strProjectName)
	{
		GUI_ASSERT(0, "not implement");
		return -1;
	}
	//------------------------------------------------------------------------------
	int32 CGUIProjectUtility::LoadPages( const CGUIString& strProjectName)
	{
		guiex::CGUIProjectInfo* pProjectInfo = guiex::CGUIProjectInfoManager::Instance()->GetProjectInfo( strProjectName );
		if( !pProjectInfo )
		{
			throw CGUIException( "[CGUIProjectUtility::LoadPages] failed to load resource by project name <%s>", strProjectName.c_str());
			return -1;
		}
		const std::vector<guiex::CGUIString>& vecPageFiles = pProjectInfo->GetWidgetFiles();
		for( std::vector<guiex::CGUIString>::const_iterator itor = vecPageFiles.begin();
			itor != vecPageFiles.end();
			++itor)
		{
			if( !guiex::CGUIWidgetSystem::Instance()->LoadPage( *itor, strProjectName ))
			{
				return -1;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}
