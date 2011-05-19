/** 
* @file guisounddata_android.cpp
* @brief 
* @author Lou Guoliang (louguoliang@gmail.com)
* @date 2011-05-10
*/

//============================================================================//
// include
//============================================================================//
#include "guisounddata_android.h"
#include <libguiex_core/guisystem.h>
#include <libguiex_core/guiscenemanager.h>
#include <libguiex_core/guilogmsgmanager.h>


//============================================================================//
// function
//============================================================================//


namespace guiex
{
	//------------------------------------------------------------------------------
	CGUISoundData_android::CGUISoundData_android( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath )
		:CGUISoundData( rName, rSceneName )
		 ,m_strPath( rPath )
		 ,m_nEffectID(-1)
	{
	}
	//------------------------------------------------------------------------------
	CGUISoundData_android::~CGUISoundData_android()
	{
	}
	//------------------------------------------------------------------------------
	int32 CGUISoundData_android::DoLoad()
	{
		CGUIString	strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;

		GUI_TRACE( GUI_FORMAT("[CGUISoundData_android::DoLoad]: load effect %s", strFullPath.c_str()));

		int nRet;
		CGUISystem::CallJavaMethod_int("org/guiex/lib/GuiexLibSound", "LoadEffect",nRet, strFullPath.c_str());
		m_nEffectID = nRet;
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUISoundData_android::DoUnload()
	{
		CGUIString	strFullPath = CGUISceneManager::Instance()->GetScenePath( m_strSceneName ) + m_strPath;

		GUI_TRACE( GUI_FORMAT("[CGUISoundData_android::DoUnload]: unload effect %s", strFullPath.c_str()));

		CGUISystem::CallJavaMethod("org/guiex/lib/GuiexLibSound", "UnloadEffect", strFullPath.c_str());
		m_nEffectID = -1;
	}
	//------------------------------------------------------------------------------
	int CGUISoundData_android::GetEffectID()
	{
		return m_nEffectID;
	}
	//------------------------------------------------------------------------------
}
