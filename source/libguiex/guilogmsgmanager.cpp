/** 
* @file guilogmsgmanager.cpp
* @brief manager of log message
* @author ken
* @date 2006-06-01
*/

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core\guilogmsgmanager.h>
#include <libguiex_core\guilogmsg.h>
#include <libguiex_core\guiobjectmanager.h>


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	//------------------------------------------------------------------------------
	GUIEXPORT void LogMessage(int32 nPriority, const CGUIString& format)
	{
		GUI_ASSERT(nPriority <= GUI_LM_MAX,"wrong parameter" );
		CGUILogMsgManager::Instance()->Msg()->Log((EGUI_Log_Priority)nPriority, format.c_str());
	}
	//------------------------------------------------------------------------------
	GUIEXPORT void PrintDebug( const CGUIString& format)
	{
		CGUILogMsgManager::Instance()->Msg()->Log( GUI_LM_DEBUG, format.c_str() );
	}
	//------------------------------------------------------------------------------
	GUI_SINGLETON_IMPLEMENT_EX(CGUILogMsgManager );
	//------------------------------------------------------------------------------
	CGUILogMsgManager::CGUILogMsgManager()
		:m_pMsgInstance(NULL)
	{

	}
	//------------------------------------------------------------------------------
	CGUILogMsgManager::~CGUILogMsgManager()
	{
	}
	//------------------------------------------------------------------------------
	CGUILogMsg*	CGUILogMsgManager::Msg()
	{
		if( m_pMsgInstance == NULL )
		{
			if( !(m_pMsgInstance = new CGUILogMsg()))
			{
				return NULL;
			}
			CGUIObjectManager::Instance()->Register(m_pMsgInstance);
		}

		return m_pMsgInstance;
	}
	//------------------------------------------------------------------------------
	CGUILogMsg*	CGUILogMsgManager::CreateMsg()
	{
		CGUILogMsg* pMsg = new CGUILogMsg();
		if( !pMsg )
		{
			return NULL;
		}
		CGUIObjectManager::Instance()->Register(pMsg);
		return pMsg;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsgManager::DestroyMsg(CGUILogMsg* pMsg)
	{
		CGUIObjectManager::Instance()->Unregister(pMsg);
	}
	//------------------------------------------------------------------------------
	void	CGUILogMsgManager::ClearDefaultMsg()
	{
		m_pMsgInstance = NULL;
	}
	//------------------------------------------------------------------------------

}
