/** 
* @file guimusicdata_android.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-
*/

//============================================================================//
// include
//============================================================================//
#include "guimusicdata_android.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guilogmsgmanager.h>


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUIMusicData_android::CGUIMusicData_android( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUIMusicData( rName, rSceneName )
		 ,m_strPath( rPath )
	{
	}
	//------------------------------------------------------------------------------
	CGUIMusicData_android::~CGUIMusicData_android()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUIMusicData_android::DoLoad()
	{	
		CGUIString	strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;

		GUI_TRACE( GUI_FORMAT("[CGUIMusicData_android::DoLoad]: load music %s", strFullPath.c_str()));

		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "LoadMusic", strFullPath.c_str());

		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUIMusicData_android::DoUnload()
	{
		CGUIString	strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;

		GUI_TRACE( GUI_FORMAT("[CGUIMusicData_android::DoUnload]: unload music %s", strFullPath.c_str()));

		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "UnloadMusic", strFullPath.c_str());
	}
	//------------------------------------------------------------------------------
}
