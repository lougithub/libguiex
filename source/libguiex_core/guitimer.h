/** 
* @file guitimer.h
* @brief time related class
* @author ken
* @date 2006-06-23
*/

#ifndef __GUI_TIMER_H_20060424__
#define __GUI_TIMER_H_20060424__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"

#if defined(GUIEX_PLATFORM_WIN32)
#	include <windows.h>
#elif defined(GUIEX_PLATFORM_LINUX)
#	include <sys/time.h>
#elif defined(GUIEX_PLATFORM_MAC)
#	include <sys/time.h>
#else
#	error "unknown platform"
#endif


//============================================================================//
// define
//============================================================================// 
//0xffffffff == 4294967295
#define GUI_TIMER_MILLISEC_MAX  4294967295
#define GUI_TIMER_SEC_MAX  4294967



//============================================================================//
// class
//============================================================================// 
namespace guiex
{
	/**
	* @class CGUITimer
	* @brief timer related function
	*/
	class GUIEXPORT CGUITimer
	{
	public:
		CGUITimer();
		CGUITimer(const CGUITimer& rTimer);
	
		void Assign( const CGUITimer& rTimer );
		void UpdateTime();

		int32 operator-(const CGUITimer& rTimer) const;
		CGUITimer operator=(const CGUITimer& rTimer);

	protected:
#if defined(GUIEX_PLATFORM_WIN32)
		static LARGE_INTEGER	m_aFrequency;
		static bool				m_sbInit;
		LARGE_INTEGER			m_aTimeCounter;
#elif defined(GUIEX_PLATFORM_LINUX)
		timeval m_aTimeVal;
#elif defined(GUIEX_PLATFORM_MAC)
		timeval m_aTimeVal;
#else
#	error "unknown platform"
#endif
	};

}//namespace guiex


#endif //__GUI_TIMER_H_20060424__
