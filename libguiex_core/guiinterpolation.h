/** 
 * @file guiinterpolation.h
 * @brief interpolation functions
 * @author ken
 * @date 2010-11-15
 */


#ifndef __GUI_INTERPOLATION_20101115__
#define __GUI_INTERPOLATION_20101115__

//============================================================================//
// include
//============================================================================// 
#include "guibase.h"
#include "guicolor.h"

namespace guiex
{
	//interpolation functions
	template<class T> 
	static T LinearTween( real t, const T& begin, const T& end )
	{
		return  end* t + begin*(1-t);
	}

	template<class T>
	static T QuadraticEaseIn( real t, const T& begin, const T& end )
	{
		return LinearTween( t*t,begin, end);
	}

	template<class T>
	static T QuadraticEaseInOut( real t, const T& begin, const T& end )
	{
		T middle = (begin+end) / 2;
		t = 2 * t;
		if( t <= 1 )
		{
			return LinearTween( t*t, begin, middle);
		}
		else
		{
			t -= 1;
			return LinearTween( t*t, middle, end);
		}
	}


	//interpolation functions
	//template<  >
	//CGUIColor LinearTween<CGUIColor>( real t, const CGUIColor& begin, const CGUIColor& end );

}//namespace guiex

#endif //__GUI_INTERPOLATION_20101115__
