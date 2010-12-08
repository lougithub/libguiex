/** 
* @file guilogmsgmanager.h
* @brief manager of log message.
* @author ken
* @date 2006-06-01
*/

#ifndef __GUI_LOGMSGMANAGER_20060601_H__
#define __GUI_LOGMSGMANAGER_20060601_H__

//============================================================================//
//	include
//============================================================================// 
#include "guiconfig.h"
#include "guitypes.h"
#include "guilogmsg.h"


//============================================================================//
// define
//============================================================================// 
/**
* @brief print a message to log
* for example: GUI_TRACE((guiex::GUI_LM_TRACE,"string",...));
*/
#define GUI_TRACE(X) \
	do { \
	::guiex::CGUILogMsg *guilog___ = ::guiex::CGUILogMsgManager::Instance()->Msg(); \
	guilog___->ConditionalSet (__FILE__, __LINE__); \
	guilog___->Log(::guiex::GUI_LM_TRACE, X) ; \
	} while (0)

/**
* @brief abbreviatation for log message
*/
#define GUI_LOG ::guiex::CGUILogMsgManager::Instance()->Msg()

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUILogMsg;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/** 
	* @class CGUILogMsgManager	
	* @brief manager of CGUILogMsg
	*/
	class GUIEXPORT CGUILogMsgManager
	{
	public:
		CGUILogMsgManager();
		~CGUILogMsgManager();

		static CGUILogMsgManager* Instance();

		CGUILogMsg*	Msg();

		CGUILogMsg*	CreateMsg();
		void DestroyMsg(CGUILogMsg*);
		void ClearDefaultMsg();

	protected:
		CGUILogMsg* m_pMsgInstance;

	private:
		static CGUILogMsgManager* m_pSingleton;
	};
}//namespace guiex


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	/**
	* @brief log message
	*/
	GUIEXPORT void LogMessage(int32 nPriority, const CGUIString& format);
	GUIEXPORT void PrintDebug( const CGUIString& format );

}//namespace guiex

#endif //__GUI_LOGMSGMANAGER_20060601_H__
