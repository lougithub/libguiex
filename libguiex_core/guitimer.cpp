/** 
* @file guitimer.cpp
* @brief time related class
* @author ken
* @date 2006-06-23
*/

//============================================================================//
// include 
//============================================================================// 
#include "guitimer.h"
#include "guiexception.h"


//============================================================================//
// function
//============================================================================// 
namespace guiex
{
#if defined(GUIEX_PLATFORM_WIN32)
	LARGE_INTEGER CGUITimer::m_aFrequency;
	bool CGUITimer::m_sbInit = false;
	//------------------------------------------------------------------------------
	/**
	* @brief constructor
	*/
	CGUITimer::CGUITimer()
	{
		m_aTimeCounter.QuadPart = 0;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief copy constructor
	*/
	CGUITimer::CGUITimer(const CGUITimer& rTimer)
		:m_aTimeCounter(rTimer.m_aTimeCounter)
	{
	}
	//------------------------------------------------------------------------------
	/**
	* @brief assign value
	*/
	void CGUITimer::Assign( const CGUITimer& rTimer )
	{
		if( this != &rTimer)
		{
			m_aTimeCounter = rTimer.m_aTimeCounter;
		}
	}
	//------------------------------------------------------------------------------
	CGUITimer CGUITimer::operator=(const CGUITimer& rTimer)
	{
		if( this != &rTimer)
		{
			m_aTimeCounter = rTimer.m_aTimeCounter;
		}
		return *this;
	}
	//------------------------------------------------------------------------------
	/**
	* @brief get current time
	*/
	void CGUITimer::UpdateTime()
	{
		if(m_aFrequency.QuadPart == 0 )
		{
			if( !QueryPerformanceFrequency(&m_aFrequency))
			{
				CGUIException::ThrowException("[CGUITimer::UpdateTime]: failed in function QueryPerformanceFrequency()!");
			}
		}
		if( !QueryPerformanceCounter(&m_aTimeCounter))
		{
			CGUIException::ThrowException("[CGUITimer::UpdateTime]: failed in function QueryPerformanceCounter()!");		
		}
	}
	//------------------------------------------------------------------------------
	int32 CGUITimer::operator-(const CGUITimer& rTimer) const 
	{
		LONGLONG newTicks = (m_aTimeCounter.QuadPart - rTimer.m_aTimeCounter.QuadPart);
		// Scale by 1000 in order to get millisecond precision
		newTicks = static_cast<LONGLONG>( newTicks /(real)m_aFrequency.QuadPart* (real)1000);

		return newTicks>GUI_TIMER_MILLISEC_MAX ? GUI_TIMER_MILLISEC_MAX :static_cast<int32>(newTicks);
	}
#elif defined(GUIEX_PLATFORM_LINUX)
	//------------------------------------------------------------------------------
	CGUITimer::CGUITimer()
	{
	}
	//------------------------------------------------------------------------------
	CGUITimer::CGUITimer(const CGUITimer& rTimer)
		:m_aTimeVal(rTimer.m_aTimeVal)
	{
	}
	//------------------------------------------------------------------------------
	void CGUITimer::Assign( const CGUITimer& rTimer )
	{
		if( this != &rTimer)
		{
			m_aTimeVal = rTimer.m_aTimeVal;
		}
	}
	//------------------------------------------------------------------------------
	CGUITimer CGUITimer::operator=(const CGUITimer& rTimer)
	{
		if( this != &rTimer)
		{
			m_aTimeVal = rTimer.m_aTimeVal;
		}
		return *this;
	}
	//------------------------------------------------------------------------------
	void CGUITimer::UpdateTime()
	{
		gettimeofday(&m_aTimeVal,NULL);
	}
	//------------------------------------------------------------------------------
	int32 CGUITimer::operator-(const CGUITimer& rTimer) const
	{
		int32 nMillSeconds = 1000 * ( m_aTimeVal.tv_sec - rTimer.m_aTimeVal.tv_sec ) 
			+ (m_aTimeVal.tv_usec - rTimer.m_aTimeVal.tv_usec)/1000;

		return nMillSeconds;
	}
#elif defined(GUIEX_PLATFORM_MAC)
	//------------------------------------------------------------------------------
	CGUITimer::CGUITimer()
	{
	}
	//------------------------------------------------------------------------------
	CGUITimer::CGUITimer(const CGUITimer& rTimer)
		:m_aTimeVal(rTimer.m_aTimeVal)
	{
	}
	//------------------------------------------------------------------------------
	void CGUITimer::Assign( const CGUITimer& rTimer )
	{
		if( this != &rTimer)
		{
			m_aTimeVal = rTimer.m_aTimeVal;
		}
	}
	//------------------------------------------------------------------------------
	CGUITimer CGUITimer::operator=(const CGUITimer& rTimer)
	{
		if( this != &rTimer)
		{
			m_aTimeVal = rTimer.m_aTimeVal;
		}
		return *this;
	}
	//------------------------------------------------------------------------------
	void CGUITimer::UpdateTime()
	{
		gettimeofday(&m_aTimeVal,NULL);
	}
	//------------------------------------------------------------------------------
	int32 CGUITimer::operator-(const CGUITimer& rTimer) const
	{
		int32 nMillSeconds = 1000 * ( m_aTimeVal.tv_sec - rTimer.m_aTimeVal.tv_sec ) 
			+ (m_aTimeVal.tv_usec - rTimer.m_aTimeVal.tv_usec)/1000;

		return nMillSeconds;
	}
#else
#	error "unknown platform"
#endif
	//------------------------------------------------------------------------------
}

