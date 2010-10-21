/** 
 * @file guilogmsgrecord.cpp
 * @brief used to store log message
 * @author ken
 * @date 2006-06-02
 */




//============================================================================//
// include
//============================================================================// 
#include <libguiex_core\guilogmsgrecord.h>
#include <libguiex_core\guiutility.h>
#include <string.h>

namespace guiex
{

//============================================================================//
// function
//============================================================================// 

//------------------------------------------------------------------------------
CGUILogMsgRecord::CGUILogMsgRecord (void)
:m_nPriority(0)
{
	m_pMsgBuf[0] = '\0';
}
//------------------------------------------------------------------------------
CGUILogMsgRecord::~CGUILogMsgRecord()
{
}
//------------------------------------------------------------------------------
const char *CGUILogMsgRecord::ms_priority_names_[] =
{
  "<none>",
  "GUI_LM_SHUTDOWN",
  "GUI_LM_TRACE",
  "GUI_LM_DEBUG",
  "GUI_LM_INFO",
  "GUI_LM_NOTICE",
  "GUI_LM_WARNING",
  "GUI_LM_STARTUP",
  "GUI_LM_ERROR",
  "GUI_LM_CRITICAL",
  "GUI_LM_ALERT",
  "GUI_LM_EMERGENCY",
  "GUI_LM_UNK(04000)",
  "GUI_LM_UNK(010000)",
  "GUI_LM_UNK(020000)",
  "GUI_LM_UNK(040000)",
  "GUI_LM_UNK(0100000)",
  "GUI_LM_UNK(0200000)",
  "GUI_LM_UNK(0400000)",
  "GUI_LM_UNK(01000000)",
  "GUI_LM_UNK(02000000)",
  "GUI_LM_UNK(04000000)",
  "GUI_LM_UNK(010000000)",
  "GUI_LM_UNK(020000000)",
  "GUI_LM_UNK(040000000)",
  "GUI_LM_UNK(0100000000)",
  "GUI_LM_UNK(0200000000)",
  "GUI_LM_UNK(0400000000)",
  "GUI_LM_UNK(01000000000)",
  "GUI_LM_UNK(02000000000)",
  "GUI_LM_UNK(04000000000)",
  "GUI_LM_UNK(010000000000)",
  "GUI_LM_UNK(020000000000)"
};
const char* CGUILogMsgRecord::GetPriorityName (EGUI_Log_Priority p)
{
	return CGUILogMsgRecord::ms_priority_names_[CGUIUtility::Log2 (p)];
}
//------------------------------------------------------------------------------
uint32 CGUILogMsgRecord::GetPriority (void) const
{
	return m_nPriority;
}
//------------------------------------------------------------------------------
void CGUILogMsgRecord::SetPriority (uint32 priority)
{
	m_nPriority = priority;
}
//------------------------------------------------------------------------------
const char * CGUILogMsgRecord::GetMsgData (void) const
{
	return (char*)m_pMsgBuf;
}
//------------------------------------------------------------------------------
size_t CGUILogMsgRecord::GetMsgDataLen (void) const
{
  return strlen ((char*)m_pMsgBuf) + 1;
}
//------------------------------------------------------------------------------
void CGUILogMsgRecord::SetMsgData (const char *data)
{
	strncpy ((char*)m_pMsgBuf, data, (sizeof m_pMsgBuf));
}
//------------------------------------------------------------------------------
//void	CGUILogMsgRecord::SetProgramName(const char* pProgName )
//{
//	if( pProgName )
//	{
//		m_strProgramName = pProgName;
//	}
//	else
//	{
//		m_strProgramName.clear();
//	}
//}
////------------------------------------------------------------------------------
//const char* CGUILogMsgRecord::GetProgramName( )
//{
//	return pProgName.c_str();
//}
////------------------------------------------------------------------------------
//void	 CGUILogMsgRecord::SetLoggerKey(const char* pLoggerKey )
//{
//	if( pLoggerKey )
//	{
//		m_strLoggerKey = pLoggerKey;
//	}
//	else
//	{
//		m_strLoggerKey.clear();
//	}
//}
////------------------------------------------------------------------------------
//const	char*	 CGUILogMsgRecord::GetLoggerKey( )
//{
//	return m_strLoggerKey.c_str();
//}
////------------------------------------------------------------------------------


}//namespace guiex
