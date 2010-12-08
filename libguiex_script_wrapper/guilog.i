namespace guiex
{
	enum EGUI_Log_Priority
	{
		GUI_LM_SHUTDOWN = 01,
		GUI_LM_TRACE = 02,
		GUI_LM_DEBUG = 04,
		GUI_LM_INFO = 010,
		GUI_LM_NOTICE = 020,
		GUI_LM_WARNING = 040,
		GUI_LM_STARTUP = 0100,
		GUI_LM_ERROR = 0200,
		GUI_LM_CRITICAL = 0400,
		GUI_LM_ALERT = 01000,
		GUI_LM_EMERGENCY = 02000,
		GUI_LM_MAX = GUI_LM_EMERGENCY,
	};

	void LogMessage( int32 nPriority, const CGUIString& format );
	void PrintDebug( const CGUIString& format );
}
