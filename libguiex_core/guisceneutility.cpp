/** 
* @file guisceneutility.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/

//============================================================================//
// include
//============================================================================//
#include <libguiex_core/guisceneutility.h>
#include <libguiex_core/guisceneinfo.h>
#include <libguiex_core/guisceneinfomanager.h>
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiwidgetmanager.h>
#include <libguiex_core/guiconfigfileloader.h>
#include <libguiex_core/guiexception.h>


//============================================================================//
// function
//============================================================================//
namespace guiex
{
	//------------------------------------------------------------------------------
	int32 CGUISceneUtility::LoadResource(const CGUIString& strSceneName)
	{
		CGUISceneInfo* pSceneInfo = CGUISceneInfoManager::Instance()->GetSceneInfo( strSceneName );
		if( !pSceneInfo )
		{
			throw CGUIException( "[CGUISceneUtility::LoadResource] failed to load resource by scene name <%s>", strSceneName.c_str());
			return -1;
		}

		if( pSceneInfo->IsResourceLoaded())
		{
			//has been loaded
			return 0;
		}
		//load this scene
		const std::vector<CGUIString>& rResourceFiles = pSceneInfo->GetResourceFiles( );
		for( uint32 i=0; i<rResourceFiles.size(); ++i )
		{
			CGUIString strResourceFilePath = pSceneInfo->GetScenePath() + rResourceFiles[i];	
			if( 0 != CGUIConfigFileLoader::LoadResourceConfigFile(strResourceFilePath, strSceneName, rResourceFiles[i]))
			{
				throw CGUIException( "[CGUISceneUtility::LoadResource] failed to load resource by scene name <%s : %s>",strResourceFilePath.c_str(), strSceneName.c_str());
				return -1;
			}
		}
		pSceneInfo->SetResourceLoaded(true);	

		//load dependencies
		const std::vector<CGUIString>&rDependencies = pSceneInfo->GetDependencies();
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
	int32 CGUISceneUtility::UnloadResource(const CGUIString& strSceneName)
	{
		GUI_ASSERT(0, "not implement");
		return -1;
	}
	//------------------------------------------------------------------------------
	int32 CGUISceneUtility::LoadPages( const CGUIString& strSceneName)
	{
		CGUISceneInfo* pSceneInfo = CGUISceneInfoManager::Instance()->GetSceneInfo( strSceneName );
		if( !pSceneInfo )
		{
			throw CGUIException( "[CGUISceneUtility::LoadPages] failed to load resource by scene name <%s>", strSceneName.c_str());
			return -1;
		}
		const std::vector<CGUIString>& vecPageFiles = pSceneInfo->GetWidgetFiles();
		for( std::vector<CGUIString>::const_iterator itor = vecPageFiles.begin();
			itor != vecPageFiles.end();
			++itor)
		{
			if( !CGUIWidgetManager::Instance()->LoadPage( *itor, strSceneName ))
			{
				return -1;
			}
		}

		return 0;
	}
	//------------------------------------------------------------------------------
}
