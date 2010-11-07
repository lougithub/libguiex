/** 
* @file guilogmsg.cpp
* @brief log system.
* @author ken
* @date 2006-05-30
*/



//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guilogmsg.h>
#include <libguiex_core/guiutility.h>
#include <libguiex_core/guilogmsgcallback.h>
#include <libguiex_core/guilogmsgrecord.h>

//============================================================================//
// function
//============================================================================// 

namespace guiex
{
#define GUI_UPDATE_COUNT(COUNT, LEN) \
	do { if (static_cast<size_t> (LEN) > COUNT) COUNT = 0; \
	 else COUNT -= static_cast<size_t> (LEN); \
	} while (0)


	//------------------------------------------------------------------------------
	uint32 CGUILogMsg::ms_nDefaultPriorityMask = 0;
	uint32 CGUILogMsg::ms_nMsgOffset = 0;
	//------------------------------------------------------------------------------
	CGUILogMsg::CGUILogMsg()
		:m_nFlags( CGUILogMsg::FLAG_STDERR)
		,m_nPriorityMask (ms_nDefaultPriorityMask)
		,m_pMsgCallback(NULL)
		,m_pOstream(NULL)
		,m_bDeleteOstream(false)
#ifdef WIN32
		,m_hEventLog(NULL)
		,m_hConsole(NULL)
#else

#endif
	{
		m_aConditionalValues.m_bIsSet  = false;
	}
	//------------------------------------------------------------------------------
	CGUILogMsg::~CGUILogMsg()
	{
		//free ostream
		if( m_bDeleteOstream && m_pOstream )
		{
			delete m_pOstream;
			m_pOstream = NULL;
		}

		//free console
#ifdef WIN32
		FreeConsole( );
		m_hConsole = NULL;
#else

#endif

		Close();
	}
	//------------------------------------------------------------------------------
	uint32 CGUILogMsg::GetFlags (void) const
	{
		uint32 result;
		result = m_nFlags;
		return result;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::SetFlags (uint32 flgs)
	{
		GUI_SET_BITS (m_nFlags, flgs);
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::ClearFlags (uint32 flgs)
	{
		GUI_CLR_BITS (m_nFlags, flgs);
	}
	//------------------------------------------------------------------------------
	uint32 CGUILogMsg::SetPriorityMask (uint32 n_mask)
	{
		uint32 o_mask = this->m_nPriorityMask;
		this->m_nPriorityMask = n_mask;

		return o_mask;
	}
	//------------------------------------------------------------------------------
	uint32 CGUILogMsg::GetPriorityMask () const
	{
		return this->m_nPriorityMask;
	}
	//------------------------------------------------------------------------------
	bool CGUILogMsg::IsPriorityEnabled (EGUI_Log_Priority log_priority) const
	{
		return GUI_BIT_ENABLED (this->m_nPriorityMask,log_priority);
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::EnableDefaultMessages (EGUI_Log_Priority priority)
	{
		GUI_SET_BITS (CGUILogMsg::ms_nDefaultPriorityMask, priority);
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::DisableDefaultMessages (EGUI_Log_Priority priority)
	{
		GUI_CLR_BITS (CGUILogMsg::ms_nDefaultPriorityMask, priority);
	}
	//------------------------------------------------------------------------------
	uint32 CGUILogMsg::GetLineNum (void) const
	{
		return this->m_nLineNum;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::SetLineNum (uint32 l)
	{
		this->m_nLineNum = l;
	}
	//------------------------------------------------------------------------------
	const char * CGUILogMsg::GetFileName (void)
	{
		return this->m_strFile;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::SetFileName (const char *s)
	{
		strncpy(this->m_strFile, s, sizeof( this->m_strFile));
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::ConditionalSet (const char *filename, uint32 line)
	{
		this->m_aConditionalValues.m_bIsSet = true;
		this->m_aConditionalValues.m_strFile = filename;
		this->m_aConditionalValues.m_nLine = line;
	}
	//------------------------------------------------------------------------------
	const char * CGUILogMsg::Msg (void)
	{
		return this->m_pMsg + CGUILogMsg::ms_nMsgOffset;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::SetCallbackMsg(CGUILogMsgCallback* pCallback)
	{
		m_pMsgCallback = pCallback;
	}
	//------------------------------------------------------------------------------
	CGUILogMsgCallback* CGUILogMsg::GetCallbackMsg() const
	{
		return m_pMsgCallback;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::Close()
	{
#ifdef WIN32
		if( m_hEventLog )
		{
			DeregisterEventSource (m_hEventLog);
			m_hEventLog = NULL;
		}
#else

#endif
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::Open (const char *prog_name,
		uint32 options_flags,
		const char *logger_key)
	{
		Close(); 

		//*****************************************************************
		//set parameter
		m_nFlags = options_flags;

		m_strProgramName.clear();
		if( prog_name )
		{
			m_strProgramName  = prog_name;
		}

		m_strLoggerKey.clear();
		if( logger_key )
		{
			m_strLoggerKey  = logger_key;
		}

		//*****************************************************************
		//for SYSLOG
		if (GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_SYSLOG))
		{
#ifdef WIN32
			char msg_file [GUI_MAXPATHLEN];
			if( m_strProgramName.empty() )
			{
				if (!::GetModuleFileNameA(NULL,msg_file,GUI_MAXPATHLEN))
				{
					return;
				}
			}
			else
			{
				strncpy( msg_file, m_strProgramName.c_str(), GUI_MAXPATHLEN );
			}
			DWORD msg_file_length =static_cast<DWORD>(strlen (msg_file) + 1);

			// If a logger_key has been supplied then we use that as the event
			// source name, otherwise we default to the program name.
			const char *event_source_name = m_strLoggerKey.empty() ? msg_file : m_strLoggerKey.c_str();

			// Information is stored in the registry at a location based on the
			// program name.
			char reg_key [GUI_MAXPATHLEN];
			strcpy (reg_key,"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
			size_t reg_key_length = strlen(reg_key);
			strncat (reg_key,event_source_name,GUI_MAXPATHLEN - reg_key_length);

			// Add the event source to the registry. Note that if this fails it
			// is not fatal. The application will still be able to write entries
			// to the event log, they just won't be formatted correctly.
			HKEY hkey = NULL;
			::RegCreateKeyA (HKEY_LOCAL_MACHINE,reg_key,&hkey);
			DWORD flags = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
			::RegSetValueExA (hkey,("TypesSupported"),0,REG_DWORD,(LPBYTE) &flags,sizeof (DWORD));
			::RegSetValueExA (hkey,("EventMessageFile"),0,REG_SZ,(LPBYTE) msg_file,msg_file_length);
			RegCloseKey (hkey);

			// Obtain a handle to the event source.
			HANDLE m_hEventLog = ::RegisterEventSourceA (0,event_source_name);
#else

#endif
		}

		//	//*****************************************************************
		//	//for STDERR
		//	if (GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_STDERR))
		//	{
		//#ifdef WIN32
		//		if( NULL == m_hConsole )
		//		{
		//			::AllocConsole();
		//			m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//		}
		//#endif
		//	}

	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::Log (EGUI_Log_Priority log_priority, const char *format_str, ...)
	{
		// Start of variable args section.
		va_list argp;

		va_start (argp, format_str);

		this->Log (format_str,log_priority,argp);
		va_end (argp);
	}
	//------------------------------------------------------------------------------
	void	CGUILogMsg::Log(const CGUILogMsgRecord& rRecord, bool bAbortProt)
	{
		// Format the message and print it to stderr and/or ship it off to
		// the log_client daemon, and/or print it to the ostream.  Of
		// course, only print the message if "FLAG_FLAG_SILENT" mode is disabled.
		if (GUI_BIT_ENABLED (m_nFlags,CGUILogMsg::FLAG_FLAG_SILENT))
		{
			return;
		}

		// Do the callback, if needed, before acquiring the lock
		// to avoid holding the lock during the callback so we don't
		// have deadlock if the callback uses the logger.
		if (GUI_BIT_ENABLED (m_nFlags,CGUILogMsg::FLAG_MSG_CALLBACK)&& GetCallbackMsg() != NULL)
		{
			GetCallbackMsg()->Log(rRecord);
		}

		// This is taken care of by our caller.
		if (GUI_BIT_ENABLED (m_nFlags,CGUILogMsg::FLAG_STDERR) && !bAbortProt) 
		{
			Log2STDERR(rRecord);
		}


		if (GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_SYSLOG))
		{
			Log2SYSLOG(rRecord);
		}

		if (GUI_BIT_ENABLED (m_nFlags,CGUILogMsg::FLAG_OSTREAM)&& GetOstream () != 0)
		{
			Log2OSTREAM(rRecord);
		}

	}

	//------------------------------------------------------------------------------
	void CGUILogMsg::Log (const char *format_str,EGUI_Log_Priority log_priority,va_list argp)
	{
		// Only print the message if <m_nPriorityMask> hasn't been reset to
		// exclude this logging priority.
		if (this->IsPriorityEnabled (log_priority) == 0)
		{
			return;
		}

		//typedef void (*PTF)(...);

		// Check if there were any conditional values set.
		bool bConditionalValues = this->m_aConditionalValues.m_bIsSet;
		this->m_aConditionalValues.m_bIsSet = false;
		if (bConditionalValues)
		{
			this->SetFileName(this->m_aConditionalValues.m_strFile);
			this->SetLineNum(this->m_aConditionalValues.m_nLine);
		}

		// bp is pointer to where to put next part of logged message.
		// bspace is the number of characters remaining in m_pMsg.
		char *bp = const_cast<char *> (this->Msg ());
		size_t bspace = GUI_MAXLOGMSGLEN;  // Leave room for Nul term.
		if (this->ms_nMsgOffset <= GUI_MAXLOGMSGLEN)
		{
			bspace -= static_cast<size_t> (this->ms_nMsgOffset);
		}

		// If this platform has snprintf() capability to prevent overrunning the
		// output buffer, use it.
		bool abort_prog = false;
		uint32 exit_value = 0;
		if (GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_VERBOSE))
		{
			// Prepend the program name onto this message
			if (!m_strProgramName.empty())
			{
				for (const char *s = m_strProgramName.c_str();
					bspace > 1 && (*bp = *s) != '\0';
					++s, --bspace)
				{
					bp++;
				}

				*bp++ = '|';
				--bspace;
			}

			//prepend the file name and linenum
			if( bConditionalValues )
			{
				for(const char *s = m_strFile;
					bspace > 1 && (*bp = *s) != '\0';
					++s, --bspace)
				{
					bp++;
				}

				*bp++ = '|';
				--bspace;

				char linebuf[32];
				snprintf( linebuf,32,"line<%d>", m_nLineNum );
				for(const char *s = linebuf;
					bspace > 1 && (*bp = *s) != '\0';
					++s, --bspace)
				{
					bp++;
				}

				*bp++ = '|';
				--bspace;
			}
		}

		if (GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_TIMESTAMP) ||
			GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_TIMESTAMP_LITE))
		{
			char day_and_time[35];
			const char *s;
			if (GUI_BIT_ENABLED (m_nFlags, CGUILogMsg::FLAG_TIMESTAMP_LITE))
			{
				// Print just the time
				s = CGUIUtility::timestamp (day_and_time, sizeof day_and_time, 1);
			}
			else
			{
				// Print time and date
				CGUIUtility::timestamp (day_and_time, sizeof day_and_time);
				s = day_and_time;
			}

			for (; bspace > 1 && (*bp = *s) != '\0'; ++s, --bspace)
			{
				++bp;
			}

			*bp++ = '|';
			--bspace;
		}

		while (*format_str != '\0' && bspace > 0)
		{
			// Copy input to output until we encounter a %, however a
			// % followed by another % is not a format specification.

			if (*format_str != '%')
			{
				*bp++ = *format_str++;
				--bspace;
			}
			else if (format_str[1] == '%') // An "escaped" '%' (just print one '%').
			{
				*bp++ = *format_str++;    // Store first %
				++format_str;             // but skip second %
				--bspace;
			}
			else
			{
				// This is most likely a format specification that ends with
				// one of the valid options described previously. To enable full
				// use of all sprintf capabilities, save the format specifier
				// from the '%' up to the format letter in a new char array.
				// This allows the full sprintf capability for padding, field
				// widths, alignment, etc.  Any width/precision requiring a
				// caller-supplied argument is extracted and placed as text
				// into the format array. Lastly, we convert the caller-supplied
				// format specifier from the CGUILogMsg-supported list to the
				// equivalent sprintf specifier, and run the new format spec
				// through sprintf, adding it to the bp string.

				const char *abort_str = ("Aborting...");
				const char *start_format = format_str;
				char format[128];			// Converted format string
				char *fp;					// Current format pointer
				uint32       wp = 0;			// Width/precision extracted from args
				uint32       done = 0;
				uint32       skip_nul_locate = 0;
				uint32       this_len = 0;		// How many chars s[n]printf wrote

				fp = format;
				*fp++ = *format_str++;   // Copy in the %

				// Initialization to satisfy VC6
				uint32 tmp_indent = 0;
				// Work through the format string to copy in the format
				// from the caller. While it's going across, extract ints
				// for '*' width/precision values from the argument list.
				// When the real format specifier is located, change it to
				// one recognized by sprintf, if needed, and do the sprintf
				// call.

				while (!done)
				{
					done = 1;               // Unless a conversion spec changes it

					switch (*format_str)
					{
						// The initial set of cases are the conversion
						// specifiers. Copy them in to the format array.
						// Note we don't use 'l', a normal conversion spec,
						// as a conversion because it is a CGUILogMsg format
						// specifier.
					case '-':
					case '+':
					case '0':
					case ' ':
					case '#':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
					case '.':
					case 'L':
					case 'h':
						*fp++ = *format_str;
						done = 0;
						break;

					case '*':
						wp = va_arg (argp, int);
						sprintf (fp, "%d", wp);
						fp += strlen (fp);
						done = 0;
						break;

					case 'a': // Abort program after handling all of format string.
						abort_prog = true;
						exit_value = va_arg (argp, int);
						strncpy (bp, abort_str, bspace);
						if (bspace > strlen (abort_str))
						{
							bspace -= strlen (abort_str);
						}
						else
						{
							bspace = 0;
						}
						break;					break;

					case 'l':             // Source file line number
						strcpy (fp, "d");
						this_len = snprintf (bp,bspace,format,GetLineNum ());
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'N':             // Source file name
						strcpy (fp, "s");
						this_len = snprintf (bp, bspace, format,GetFileName () ?GetFileName (): "<unknown file>");
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'n':             // Program name
						strcpy (fp, "s");
						this_len = snprintf (bp, bspace, format,m_strProgramName.empty() ?
							"<unknown>":m_strProgramName.c_str());
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'M': // Print the name of the priority of the message.
						strcpy (fp, "s");
						this_len = snprintf(bp, bspace, format,
							CGUILogMsgRecord::GetPriorityName (log_priority));
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'D': // Format the timestamp in month/day/year hour:minute:sec:usec format.
						{
							char day_and_time[35];
							CGUIUtility::timestamp (day_and_time,sizeof day_and_time);
							strcpy (fp, "s");
							this_len = snprintf(bp, bspace, format, day_and_time);
							GUI_UPDATE_COUNT (bspace, this_len);
							break;
						}

					case 'T': // Format the timestamp in hour:minute:sec:usec format.
						{
							char day_and_time[35];
							strcpy (fp, "s");
							this_len = snprintf(bp, bspace, format,
								CGUIUtility::timestamp (day_and_time, sizeof day_and_time));
							GUI_UPDATE_COUNT (bspace, this_len);
							break;
						}

					case 's':                       // String
						strcpy (fp, "s");
						this_len = snprintf(bp, bspace, format, va_arg (argp, char *));
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'c':
						strcpy (fp, "C");
						this_len = snprintf(bp, bspace, format, va_arg (argp, int));
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'd': case 'i': case 'o':
					case 'u': case 'x': case 'X':
						fp[0] = *format_str;
						fp[1] = '\0';
						this_len = snprintf(bp, bspace, format, va_arg (argp, int));
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'F': case 'f': case 'e': case 'E':
					case 'g': case 'G':
						fp[0] = *format_str;
						fp[1] = '\0';
						this_len = snprintf
							(bp, bspace, format, va_arg (argp, double));
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					case 'Q':
						{
							const char *fmt = PRINTF_UINT64;
							strcpy (fp, &fmt[1]);    // Skip leading %
							this_len = snprintf (bp, bspace,fmt,va_arg (argp, uint64));
							GUI_UPDATE_COUNT (bspace, this_len);
							break;
						}

					case 'q':
						{
							const char *fmt = PRINTF_INT64;
							strcpy (fp, &fmt[1]);    // Skip leading %
							this_len = snprintf (bp, bspace,format,va_arg (argp, int64));
						}
						GUI_UPDATE_COUNT (bspace, this_len);

					case '@':
						strcpy (fp, "p");
						this_len = snprintf(bp, bspace, format, va_arg (argp, void*));
						GUI_UPDATE_COUNT (bspace, this_len);
						break;

					default:
						// So, it's not a legit format specifier after all...
						// Copy from the original % to where we are now, then
						// continue with whatever comes next.
						while (start_format != format_str && bspace > 0)
						{
							*bp++ = *start_format++;
							--bspace;
						}
						if (bspace > 0)
						{
							*bp++ = *format_str;
							--bspace;
						}
						break;
					}

					// Bump to the next char in the caller's format_str
					++format_str;
				}

				if (!skip_nul_locate)
				{
					while (*bp != '\0') // Locate end of bp.
					{
						++bp;
					}
				}
			}
		}

		*bp = '\0'; // Terminate bp, but don't auto-increment this!

		// Check that memory was not corrupted.
		if (bp >= this->m_pMsg + sizeof this->m_pMsg)
		{
			abort_prog = true;
			fprintf (stderr,"The following logged message is too long!\n");
		}

		CGUILogMsgRecord aLogRecord;
		aLogRecord.SetPriority( this->m_nPriorityMask);
		aLogRecord.SetMsgData (this->Msg ());

		// Write the <rRecord> to the appropriate location.
		this->Log( aLogRecord, abort_prog);

		if (abort_prog)
		{
			// *Always* print a message to stderr if we're aborting.  We
			// don't use verbose, however, to avoid recursive aborts if
			// something is hosed.
			//rRecord.print (CGUILogMsg::local_host_, 0, stderr);
			::abort ();
		}
	}
	//------------------------------------------------------------------------------
	uint32 CGUILogMsg::LogHexDump (EGUI_Log_Priority log_priority, 
		const char *buffer,
		size_t size,
		const char *text)
	{
		char buf[CGUILogMsgRecord::MAXLOGMSGLEN -CGUILogMsgRecord::VERBOSE_LEN - 58];
		// 58 for the HEXDUMP header;

		const size_t text_sz = text ? strlen(text) : 0;
		char *msg_buf = new char[text_sz+58];
		if( !msg_buf)
		{
			return -1;
		}
		buf[0] = 0; // in case size = 0

		const size_t len = CGUIUtility::format_hexdump(
			buffer, size, buf, sizeof (buf) / sizeof (char) - text_sz);

		int sz = 0;

		if (text)
		{
			sz = sprintf (msg_buf,"%s - ",text);
		}

		sz += sprintf (msg_buf + sz,"HEXDUMP %u bytes",size);

		if (len < size)
		{
			sprintf (msg_buf + sz," (showing first %u bytes)",len);
		}

		// Now print out the formatted buffer.
		this->Log (log_priority,"%s\n%s",msg_buf,buf);

		delete [] msg_buf;
		return 0;
	}
	//------------------------------------------------------------------------------
	void CGUILogMsg::Cleanup ()
	{
		delete this;
	}
	//------------------------------------------------------------------------------
	void	CGUILogMsg::Log2STDERR(const CGUILogMsgRecord& rRecord)
	{
#ifdef WIN32
		if (IsDebuggerPresent()) 
		{
			//Sleep(1);
			OutputDebugStringA(rRecord.GetMsgData());
		}
		else
		{
			if( !m_hConsole )
			{
				::AllocConsole();
				m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			}

			DWORD dwWritten=0;
			DWORD dwBytesToWrite = DWORD(strlen( rRecord.GetMsgData()));

			//set new color

			WriteConsole( m_hConsole, rRecord.GetMsgData(), dwBytesToWrite, &dwWritten, NULL );
		}
#else
		fprintf (stderr, "%s", rRecord.GetMsgData());
#endif
	}
	//------------------------------------------------------------------------------
	void	CGUILogMsg::Log2SYSLOG(const CGUILogMsgRecord& rRecord)
	{
#ifdef WIN32
		if( !m_hEventLog )
		{
			char msg_file [GUI_MAXPATHLEN];
			if( m_strProgramName.empty() )
			{
				if (!::GetModuleFileNameA(NULL,msg_file,GUI_MAXPATHLEN))
				{
					return;
				}
			}
			else
			{
				strncpy( msg_file, m_strProgramName.c_str(), GUI_MAXPATHLEN );
			}
			DWORD msg_file_length =static_cast<DWORD>(strlen (msg_file) + 1);

			// If a logger_key has been supplied then we use that as the event
			// source name, otherwise we default to the program name.
			const char *event_source_name = m_strLoggerKey.empty() ? msg_file : m_strLoggerKey.c_str();

			// Information is stored in the registry at a location based on the
			// program name.
			char reg_key [GUI_MAXPATHLEN];
			strcpy (reg_key,"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
			size_t reg_key_length = strlen(reg_key);
			strncat (reg_key,event_source_name,GUI_MAXPATHLEN - reg_key_length);

			// Add the event source to the registry. Note that if this fails it
			// is not fatal. The application will still be able to write entries
			// to the event log, they just won't be formatted correctly.
			HKEY hkey = NULL;
			::RegCreateKeyA (HKEY_LOCAL_MACHINE,reg_key,&hkey);
			DWORD flags = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
			::RegSetValueExA (hkey,("TypesSupported"),0,REG_DWORD,(LPBYTE) &flags,sizeof (DWORD));
			::RegSetValueExA (hkey,("EventMessageFile"),0,REG_SZ,(LPBYTE) msg_file,msg_file_length);
			RegCloseKey (hkey);

			// Obtain a handle to the event source.
			HANDLE m_hEventLog = ::RegisterEventSourceA (0,event_source_name);
		}


		// Make a copy of the log text and replace any newlines with
		// CR-LF. Newline characters on their own do not appear correctly
		// in the event viewer. We allow for a doubling in the size of
		// the msg data for the worst case of all newlines.
		const char* src_msg_data = rRecord.GetMsgData();
		char msg_data [CGUILogMsgRecord::MAXLOGMSGLEN * 2];

		size_t len = rRecord.GetMsgDataLen ();
		for (size_t i = 0, j = 0; i < len; ++i)
		{
			if (src_msg_data[i] == '\n')
			{
				msg_data[j++] = '\r';
				msg_data[j++] = '\n';
			}
			else
			{
				msg_data[j++] = src_msg_data[i];
			}
		}

		// Map the ACE log record type to an event log type.
		WORD event_type;
		switch (rRecord.GetPriority())
		{
		case GUI_LM_STARTUP:
		case GUI_LM_SHUTDOWN:
		case GUI_LM_TRACE:
		case GUI_LM_DEBUG:
		case GUI_LM_INFO:
			event_type = EVENTLOG_INFORMATION_TYPE;
			break;
		case GUI_LM_NOTICE:
		case GUI_LM_WARNING:
			event_type = EVENTLOG_WARNING_TYPE;
			break;
		case GUI_LM_ERROR:
		case GUI_LM_CRITICAL:
		case GUI_LM_ALERT:
		case GUI_LM_EMERGENCY:
		default:
			event_type = EVENTLOG_ERROR_TYPE;
			break;
		}

		// Send the log message to the system event log.
		const char* msgs [1];
		msgs[0] = msg_data;

		::ReportEventA(m_hEventLog,event_type, 0, 0, 0, 1, 0, msgs, 0);
#else

#endif //#ifdef WIN32
	}
	//------------------------------------------------------------------------------
	void	CGUILogMsg::Log2OSTREAM(const CGUILogMsgRecord& rRecord)
	{
		if( !GetOstream())
		{
			return;
		}
		std::ostream& s = *GetOstream();
		s << rRecord.GetMsgData();
		s.flush ();
	}
	//------------------------------------------------------------------------------
	void	CGUILogMsg::SetOstream( std::ostream* pOstream, bool bDelete/*=false*/ )
	{
		m_pOstream = pOstream;
		m_bDeleteOstream = bDelete;
	}
	//------------------------------------------------------------------------------
	std::ostream*	CGUILogMsg::GetOstream() const
	{
		return m_pOstream;
	}
	//------------------------------------------------------------------------------

}//namespace guiex
