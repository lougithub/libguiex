/** 
* @file guiassert.cpp
* @brief Creates a Macro for Assert
* @author ken
* @date 2007-05-09
*/



//============================================================================//
// include
//============================================================================// 
#include "guiassert.h"
#include "guilogmsgmanager.h"
#include "guilogmsg.h"

namespace guiex
{
	//============================================================================//
	// function
	//============================================================================// 
	//------------------------------------------------------------------------------
	void*			CGUIAssert::ms_AssertUserData = NULL;
	//------------------------------------------------------------------------------
	FGUIAssertCB		CGUIAssert::ms_AssertCB = NULL;
	//------------------------------------------------------------------------------
	void*			CGUIAssert::ms_WarningUserData = NULL;
	//------------------------------------------------------------------------------
	FGUIWarningCB	CGUIAssert::ms_WarningCB = NULL;
	//------------------------------------------------------------------------------
	char         CGUIAssert::ms_msg[1024];
	//------------------------------------------------------------------------------
	void CGUIAssert::SetAssertCB(FGUIAssertCB cb, void* user_data)
	{
		ms_AssertUserData = user_data;
		ms_AssertCB = cb;
	}
	//------------------------------------------------------------------------------
	void CGUIAssert::SetWarningCB(FGUIWarningCB cb, void* user_data)
	{
		ms_WarningUserData = user_data;
		ms_WarningCB = cb;
	}
	//------------------------------------------------------------------------------
	void CGUIAssert::Assert(const char* file, int32 line, const char* message, const char* expression)
	{
		::guiex::CGUILogMsg *guilog = ::guiex::CGUILogMsgManager::Instance()->Msg();
		guilog->ConditionalSet (file, line); 
		guilog->Log(::guiex::GUI_LM_ERROR, "GUI_ASSERT: (%s), <file %N, line %l> assertion failed for '%s'.\n",message, expression);

		if (ms_AssertCB)
		{
			ms_AssertCB(file, line, message, expression, ms_AssertUserData);
		}

#ifdef WIN32
		DebugBreak();
#endif
		::abort ();
	}
	//------------------------------------------------------------------------------
	void CGUIAssert::Warning(const char* file, int32 line,const char* message)
	{
		::guiex::CGUILogMsg *guilog = ::guiex::CGUILogMsgManager::Instance()->Msg();
		guilog->ConditionalSet (file, line); 
		guilog->Log(::guiex::GUI_LM_WARNING, "GUI_WARNING: <file %N, line %l>  %s.\n",message);

		if (ms_WarningCB)
		{
			ms_WarningCB( message,ms_WarningUserData);
		}
	}
	//------------------------------------------------------------------------------
	void CGUIAssert::WarningHold(const char* file, int32 line,const char* message)
	{
		::guiex::CGUILogMsg *guilog = ::guiex::CGUILogMsgManager::Instance()->Msg();
		guilog->ConditionalSet (file, line); 
		guilog->Log(::guiex::GUI_LM_WARNING, "GUI_WARNING_HOLD: <file %N, line %l>  %s.\n",message);

		if (ms_WarningCB)
		{
			ms_WarningCB( message,ms_WarningUserData);
		}

#ifdef WIN32
		DebugBreak();
#endif
	}
	//------------------------------------------------------------------------------
	const char* CGUIAssert::Format(const char* message, ...)
	{
		if (!message) 
		{
			*ms_msg = '\0';
		}
		else
		{
			va_list arglist;
			va_start(arglist, message);
			vsprintf(ms_msg, message, arglist);
			va_end(arglist);
		}

		return ms_msg;
	}
	//------------------------------------------------------------------------------
}//namespace guiex
