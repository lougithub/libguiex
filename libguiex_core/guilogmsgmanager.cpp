/** 
* @file guilogmsgmanager.cpp
* @brief manager of log message
* @author ken
* @date 2006-06-01
*/

//============================================================================//
// include 
//============================================================================// 
#include "guilogmsgmanager.h"
#include "guilogmsg.h"
#include "guiobjectmanager.h"


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
	CGUILogMsgManager* CGUILogMsgManager::m_pSingleton = NULL;
	//------------------------------------------------------------------------------
	CGUILogMsgManager::CGUILogMsgManager()
		:m_pMsgInstance(NULL)
	{
		GUI_ASSERT( !m_pSingleton, "[CGUILogMsgManager::CGUILogMsgManager]:instance has been created" ); 
		m_pSingleton = this; 
	}
	//------------------------------------------------------------------------------
	CGUILogMsgManager::~CGUILogMsgManager()
	{
		m_pSingleton = NULL;
	}
	//------------------------------------------------------------------------------
	CGUILogMsgManager* CGUILogMsgManager::Instance()
	{
		return m_pSingleton;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get default message.
	*/
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
	/**
	* @brief generate a message
	*/
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
	/**
	* @brief generate a message
	*/
	void CGUILogMsgManager::DestroyMsg(CGUILogMsg* pMsg)
	{
		CGUIObjectManager::Instance()->Unregister(pMsg);
	}
	//------------------------------------------------------------------------------
	/** 
	* @brief clear default msg instance
	*/
	void CGUILogMsgManager::ClearDefaultMsg()
	{
		m_pMsgInstance = NULL;
	}
	//------------------------------------------------------------------------------

}
