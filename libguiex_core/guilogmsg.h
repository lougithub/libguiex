/** 
 * @file guilogmsg.h
 * @brief log system.
 * @author ken
 * @date 2006-05-30
 */

#ifndef __GUI_LOGMSG_20060530_H__
#define __GUI_LOGMSG_20060530_H__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guistring.h"
#include "guisingleton.h"
#include <string>
#include <stdarg.h>


#ifdef WIN32
#	ifndef _WIN32_WINDOWS
#		define _WIN32_WINDOWS 0x0500
#	endif
#	include <windows.h>
#else

#endif

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUILogMsgCallback;
	class CGUILogMsgRecord;
}//namespace guiex


//============================================================================//
// enum
//============================================================================// 
namespace guiex
{

	enum EGUI_Log_Priority
	{
		// = Note, this first argument *must* start at 1!

		/// Shutdown the logger(decimal 1).
		GUI_LM_SHUTDOWN = 01,

		/// Messages indicating function-calling sequence(decimal 2).
		GUI_LM_TRACE = 02,

		/// Messages that contain information normally of use only when
		/// debugging a program(decimal 4).
		GUI_LM_DEBUG = 04,

		/// Informational messages(decimal 8).
		GUI_LM_INFO = 010,

		/// Conditions that are not error conditions, but that may require
		/// special handling(decimal 16).
		GUI_LM_NOTICE = 020,

		/// Warning messages(decimal 32).
		GUI_LM_WARNING = 040,

		/// Initialize the logger(decimal 64).
		GUI_LM_STARTUP = 0100,

		/// Error messages(decimal 128).
		GUI_LM_ERROR = 0200,

		/// Critical conditions, such as hard device errors(decimal 256).
		GUI_LM_CRITICAL = 0400,

		/// A condition that should be corrected immediately, such as a
		/// corrupted system database(decimal 512).
		GUI_LM_ALERT = 01000,

		/// A panic condition.  This is normally broadcast to all users
		///(decimal 1024).
		GUI_LM_EMERGENCY = 02000,

		/// The maximum logging priority.
		GUI_LM_MAX = GUI_LM_EMERGENCY,

		/// Do not use!!  This enum value ensures that the underlying
		/// integral type for this enum is at least 32 bits.
		GUI_LM_ENSURE_32_BITS = 0x7FFFFFFF
	};
}//namespace guiex


//============================================================================//
// class
//============================================================================//
namespace guiex
{
	/**
	* @class CGUILogMsg
	* @brief used to log message.this is a singleton class, you should 
	* use GUI_LOG_MSG instead of CGUILogMsg.
	*/
	class GUIEXPORT CGUILogMsg  : public CGUICleanup
	{
	public:
		// Logger Flags.
		enum
		{
			/// Write messages to stderr.
			FLAG_STDERR = 1,
			/// Write messages to the ostream * stored 
			FLAG_OSTREAM = 2,
			/// Write messages to the callback object.
			FLAG_MSG_CALLBACK = 4,
			/// Display messages in a verbose manner.
			FLAG_VERBOSE = 8,
			/// Display messages in a less verbose manner(i.e., only print
			/// information that can change between calls).
			FLAG_VERBOSE_LITE = 16,
			// show timestamp, include time and date
			FLAG_TIMESTAMP = 32,
			// show timestamp, only time
			FLAG_TIMESTAMP_LITE = 64,
			/// Do not print messages at all(just leave in thread-specific
			/// storage for later inspection).
			FLAG_FLAG_SILENT = 128,
			/// Write messages to the system's event log.
			FLAG_SYSLOG = 256
		};

		void SetFlags(uint32 f);
		void ClearFlags(uint32 f);
		uint32 GetFlags(void) const;

		uint32 GetPriorityMask() const;
		uint32 SetPriorityMask(uint32);
		bool IsPriorityEnabled(EGUI_Log_Priority log_priority) const;

		static void DisableDefaultMessages(EGUI_Log_Priority priority = GUI_LM_DEBUG);
		static void EnableDefaultMessages(EGUI_Log_Priority priority = GUI_LM_DEBUG);

		void SetLineNum(uint32);
		uint32 GetLineNum(void) const;

		void SetFileName(const char *);
		const char *GetFileName(void);

		void ConditionalSet(const char *file, uint32 line );

		void Log(EGUI_Log_Priority priority, const char *format, ...);
		void Log(const char *format,EGUI_Log_Priority priority,va_list argp);

		void SetCallbackMsg(CGUILogMsgCallback*);
		CGUILogMsgCallback* GetCallbackMsg() const;

		const char * Msg(void);

		void SetOstream( std::ostream* pOstream, bool bDelete=false );
		std::ostream* GetOstream() const;

		uint32 LogHexDump(EGUI_Log_Priority log_priority, const char *buffer,size_t size,const char *text = 0);

		void Open(const char *prog_name,
			uint32 options_flags = CGUILogMsg::FLAG_STDERR,
			const char *logger_key = NULL);
		void Close();
		virtual void Cleanup();

	protected:
		void Log(const CGUILogMsgRecord& rRecord, bool bAbortProt);
		void Log2SYSLOG(const CGUILogMsgRecord& rRecord);
		void Log2STDERR(const CGUILogMsgRecord& rRecord);
		void Log2OSTREAM(const CGUILogMsgRecord& rRecord);

	protected:
		friend class CGUILogMsgManager;
		CGUILogMsg();
		~CGUILogMsg();
		
	protected:
		/**
		 * Options flags used to hold the logger flag options, e.g.,
		 * FLAG_STDERR, FLAG_OSTREAM, FLAG_MSG_CALLBACK, etc.
		 */
		uint32 m_nFlags;
		
		/**
		 * Keeps track of all the per-thread EGUI_Log_Priority values that
		 * are currently enabled.  Default is for all logging priorities to
		 * be disabled.
		 */
		uint32 m_nPriorityMask;
		
		/**
		 * Priority mask to use for each new instance
		 */
		static uint32 ms_nDefaultPriorityMask;
		
		/// Anonymous struct since there will only be one instance.  This
		/// struct keeps information stored away in case we actually end up
		/// calling log() if the log priority is correct.
		struct
		{
			bool m_bIsSet;
			const char * m_strFile;
			uint32 m_nLine;
		} m_aConditionalValues;
		
		/// Line number where the error occurred.
		uint32 m_nLineNum;
		
		/// File where the error occurred.
		char m_strFile[GUI_MAXPATHLEN + 1];
		
		CGUILogMsgCallback* m_pMsgCallback;
		
		/// Records the program name.
		std::string m_strProgramName;
		
		/// Records the logger key.
		std::string m_strLoggerKey;
		
		
		/**
		 * The log message, which resides in storage.  Note that only 
		 * the current log message is stored here -- it will be
		 * overwritten by the subsequent call to Log().
		 */
		char m_pMsg[GUI_MAXLOGMSGLEN + 1]; // Add one for NUL-terminator.
		
		/**
		 * Offset of m_pMsg[].
		 */
		static uint32 ms_nMsgOffset;
		
		/// Are we deleting this ostream?
		bool m_bDeleteOstream;
		
		/// The ostream where logging messages can be written.
		std::ostream* m_pOstream;
		
#ifdef WIN32
		HANDLE m_hEventLog;
		HANDLE m_hConsole;
#else

#endif //#ifdef WIN32
		
	};

}//namespace guiex
#endif //__GUI_LOGMSG_20060530_H__
