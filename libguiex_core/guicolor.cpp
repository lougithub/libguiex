/** 
* @file guicolor.cpp
* @brief implement of class CGUIColor.
* @author ken
* @date 2006-03-23
*/

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guicolor.h>
#include <algorithm>

//============================================================================//
// function
//============================================================================// 
namespace guiex
{
	//------------------------------------------------------------------------------
	/** 
	* @brief default constructor
	*/
	CGUIColor::CGUIColor()
		:m_nARGB(0xFF000000)
	{
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief copy constructor
	*/
	CGUIColor::CGUIColor(const CGUIColor &rColor)
	{
		SetColor(rColor);
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief constructor
	* @param argb color value, a 4-bytes number
	*/
	CGUIColor::CGUIColor(GUIARGB argb)
	{
		SetARGB(argb);
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief constructor
	* @param fRed red factor of color, from 0.0 to 1.0
	* @param fGreen green factor of color, from 0.0 to 1.0
	* @param fBlue blue factor of color, from 0.0 to 1.0
	* @param fAlpha alpha factor of color, from 0.0 to 1.0, default is 1.0f
	*/
	CGUIColor::CGUIColor(real fRed, real fGreen, real fBlue, real fAlpha /*=1.0f*/ )
	{
		SetColor(fRed, fGreen, fBlue, fAlpha);
	}
	//------------------------------------------------------------------------------ 
	/** 
	* @brief convert color from ARGB to RGBA
	* @return color value with 4-bytes number
	*/
	GUIRGBA CGUIColor::ARGB2RGBA( GUIARGB aColorARGB )
	{
		return ((aColorARGB<<8) + ((aColorARGB>>24)&0xFF));
	}
	//------------------------------------------------------------------------------ 

}//namespace guiex

