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

namespace guiex
{
//============================================================================//
// enum
//============================================================================// 
//============================================================================//
// class
//============================================================================// 
class 	GUIEXPORT CGUILogMsgRecord
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

	/**
	 * @brief Get the priority of the <Log_Record> <type_>.  This is computed
	 * as the base 2 logarithm of <type_> (which must be a power of 2,
	 * as defined by the enums in <GUI_Log_Priority>).
	 */
	uint32 GetPriority (void) const;

	/**
	 * @brief get priority name in character
	 */
	static const char* GetPriorityName (EGUI_Log_Priority p);

	///**
	// * @brief set program name
	// */
	//void	SetProgramName(const char* pProgName );

	///**
	// * @brief get program name
	// */
	//const	char*	GetProgramName( );

	///**
	// * @brief set logger key
	// */
	//void	SetLoggerKey(const char* pLoggerKey );

	///**
	// * @brief get logger key
	// */
	//const	char*	GetLoggerKey( );

	/**
	 * @brief Set the priority of the <Log_Record> <type_> (which must be a
	 * power of 2, as defined by the enums in <GUI_Log_Priority>).
	 */
	void SetPriority (uint32 num);

	///get the length of message data
	size_t GetMsgDataLen (void) const;

	/// Get the message data of the <Log_Record>.
	const char * GetMsgData (void) const;

	/// Set the message data of the <Log_Record>.
	void SetMsgData (const char *data);


	/**
	 * @brief get the length of this record
	 */
	//int32	GetLength();



protected:
	/// Logging record data
	char*	m_pMsgBuf[MAXLOGMSGLEN + 1];

	/// priority of logging record.
	uint32		m_nPriority;

	/////program name
	//std::string m_strProgramName;

	/////logger key
	//std::string m_strLoggerKey;

	/// Symbolic names for the <EGUI_Log_Priority> enums.
	static const char *ms_priority_names_[];
};




}//namespace guiex

#endif //__GUI_LOGMSGRECORD_20060602_H__
