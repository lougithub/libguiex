/** 
* @file guisceneutility.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2009-10-22
*/

//============================================================================//
// include
//============================================================================//
#include "guisceneutility.h"
#include "guiscene.h"
#include "guiscenemanager.h"
#include "guisystem.h"
#include "guiwidgetmanager.h"
#include "guiconfigfileloader.h"
#include "guiexception.h"


//============================================================================//
// function
//============================================================================//
namespace guiex
{
	//------------------------------------------------------------------------------
	int32 CGUISceneUtility::LoadPages( const CGUIString& strSceneName)
	{
		CGUIScene* pSceneInfo = CGUISceneManager::Instance()->GetScene( strSceneName );
		if( !pSceneInfo )
		{
			CGUIException::ThrowException( "[CGUISceneUtility::LoadPages] failed to load resource by scene name <%s>", strSceneName.c_str());
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
