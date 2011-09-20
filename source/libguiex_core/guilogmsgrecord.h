/** 
* @file guilogmsgrecord.h
* @brief used to store log message
* @author ken
* @date 2006-06-02
*/

#ifndef __GUI_LOGMSGRECORD_20060602_H__
#define __GUI_LOGMSGRECORD_20060602_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include <string>
#include "guilogmsg.h"

//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	class GUIEXPORT CGUILogMsgRecord
	{
	public:
		enum
		{
			/// Maximum size of a logging message.
			MAXLOGMSGLEN = GUI_MAXLOGMSGLEN,

			/// Most restrictive alignment.
			ALIGN_WORDB  = 8,

			/// Size used by verbose mode.
			/// 20 (date) + 15 (host_name) + 10 (pid) + 10 (type)
			///           + 4 (@) ... + ? (progname)
			VERBOSE_LEN = 128,

			/// Maximum size of a logging message with the verbose headers
			MAXVERBOSELOGMSGLEN = VERBOSE_LEN + MAXLOGMSGLEN
		};

		CGUILogMsgRecord (void);
		~CGUILogMsgRecord();

		void SetPriority (uint32 num);
		uint32 GetPriority (void) const;
		static const char* GetPriorityName (EGUI_Log_Priority p);

		size_t GetMsgDataLen (void) const;
		const char * GetMsgData (void) const;
		void SetMsgData (const char *data);

	protected:
		/// Logging record data
		char* m_pMsgBuf[MAXLOGMSGLEN + 1];

		/// priority of logging record.
		uint32 m_nPriority;

		/// Symbolic names for the <EGUI_Log_Priority> enums.
		static const char *ms_priority_names_[];
	};

}//namespace guiex

#endif //__GUI_LOGMSGRECORD_20060602_H__
