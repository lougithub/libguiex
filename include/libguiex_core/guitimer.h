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

#if defined(__WIN32__) || defined(_WIN32)
#	include <windows.h>
#elif defined(__linux__)
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
		/**
		* @brief constructor
		*/
		CGUITimer();

		/**
		* @brief copy constructor
		*/
		CGUITimer(const CGUITimer& rTimer);

		/**
		* @brief assign value
		*/
		void	Assign( const CGUITimer& rTimer );

		/**
		* @brief assign operation
		*/
		CGUITimer operator=(const CGUITimer& rTimer);

		/**
		* @brief get current time
		*/
		void	UpdateTime();

		/**
		* @brief get timerinterval, in millisecond
		*/
		int32	operator-(const CGUITimer& rTimer) const;


	protected:
#if defined(__WIN32__) || defined(_WIN32)
		static LARGE_INTEGER	m_aFrequency;
		static bool				m_sbInit;
		LARGE_INTEGER			m_aTimeCounter;
#elif defined(__linux__)
		timeval m_aTimeVal;
#else
#	error "unknown platform"
#endif
	};

}//namespace guiex


#endif //__GUI_TIMER_H_20060424__
