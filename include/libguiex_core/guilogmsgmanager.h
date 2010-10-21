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
#include "guisingleton.h"



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
		/**
		* @brief get default message.
		*/
		CGUILogMsg*	Msg();

		/**
		* @brief generate a message
		*/
		CGUILogMsg*	CreateMsg();

		/**
		* @brief generate a message
		*/
		void DestroyMsg(CGUILogMsg*);

		/** 
		* @brief clear default msg instance
		*/
		void	ClearDefaultMsg();

	protected:
		CGUILogMsgManager();
		~CGUILogMsgManager();

	protected:
		CGUILogMsg* m_pMsgInstance;

		//declare for singleton
		GUI_SINGLETON_DECLARE_EX(CGUILogMsgManager );

	};

	//implement for singleton
	//GUI_SINGLETON_IMPLEMENT(CGUILogMsgManager, CGUILogMsgManager );

}//namespace guiex


//============================================================================//
// function
//============================================================================// 
namespace guiex
{

	/**
	* @brief log message
	*/
	GUIEXPORT  void LogMessage(int32 nPriority, const CGUIString& format);
	GUIEXPORT  void PrintDebug( const CGUIString& format );

}//namespace guiex

#endif //__GUI_LOGMSGMANAGER_20060601_H__
