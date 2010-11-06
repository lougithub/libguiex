/** 
 * @file guiexception.cpp
 * @brief exception used in this system
 * @author ken
 * @date 2006-05-30
 */

//============================================================================//
// include 
//============================================================================// 
#include <libguiex_core/guiexception.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32
#	ifndef _WIN32_WINDOWS
#		define _WIN32_WINDOWS 0x0500
#	endif
#	include <windows.h>
#endif


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	void ThrowException(const char* szError)
	{
		throw CGUIException(szError);
	}
	//------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------------
	CGUIBaseException::CGUIBaseException( const char* szError ) throw()
	:std::exception()
	,m_strError(szError)
	{
	}
	//------------------------------------------------------------------------------
	const char * CGUIBaseException::what( ) const throw()
	{
		return m_strError.c_str();
	}
	//------------------------------------------------------------------------------
	
	
	//------------------------------------------------------------------------------
	CGUIException::CGUIException( const char *format, ... ) throw()
	{
		char szBuffer[4097];
		va_list argp;
		va_start (argp, format);
		::vsnprintf( szBuffer, 4096, format, argp );
		m_strError = szBuffer;
		va_end(argp);
		
#if GUI_EXCEPTION_ASSERT
#	ifdef WIN32
		if (IsDebuggerPresent()) 
		{
			GUI_FORCE_ASSERT(m_strError.c_str());
		}
#	endif
#endif
	}
	
	//------------------------------------------------------------------------------

	
	
	//------------------------------------------------------------------------------
	CGUIException_Script::CGUIException_Script( const char *format, ... ) throw()
	{
		char szBuffer[4097];
		va_list argp;
		va_start (argp, format);
		::vsnprintf( szBuffer, 4096, format, argp );
		m_strError = szBuffer;
		va_end(argp);
	}
	//------------------------------------------------------------------------------
}//namespace guiex
