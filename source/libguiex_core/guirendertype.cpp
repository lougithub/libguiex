/** 
* @file guirendertype.cpp
* @brief 
* @author ken
* @date 2011-01-14
*/

//============================================================================//
// include
//============================================================================// 
#include "guirendertype.h"
#include "guicolor.h"

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	void ConvGUIColor_2_C4f( const class CGUIColor& rSrc, SR_C4F& rDst )
	{
		rDst.r = rSrc.GetRed();
		rDst.g = rSrc.GetGreen();
		rDst.b = rSrc.GetBlue();
		rDst.a = rSrc.GetAlpha();
	}

}

