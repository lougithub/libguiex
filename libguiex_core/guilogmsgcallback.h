/** 
* @file guilogmsgcallback.h
* @brief callback class that allow user to 
* control how to log message
* @author ken
* @date 2006-06-02
*/

#ifndef __GUI_LOGMSGCALLBACK_20060602_H__
#define __GUI_LOGMSGCALLBACK_20060602_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUILogMsgRecord;
}

//============================================================================//
// class
//============================================================================// 
namespace guiex
{

	/**
	* @class CGUILogMsgCallback
	* @brief An interface class used to get logging callbacks.
	*
	*     Users who are interested in getting the logging messages
	*     directly, can subclass this interface and override the Log()
	*     method. They must then register their subclass with the
	*     Log_Msg class and make sure that they turn on the
	*     CGUILogMsg::FLAG_MSG_CALLBACK flag.
	*/
	class GUIEXPORT CGUILogMsgCallback
	{
	public:
		virtual ~CGUILogMsgCallback();

		virtual void	Log( const CGUILogMsgRecord& ) = 0;
	};

}//namespace guiex

#endif //__GUI_LOGMSGCALLBACK_20060602_H__
