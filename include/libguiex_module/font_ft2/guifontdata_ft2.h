/** 
* @file guifontdata_ft2.h
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/

#ifndef __GUI_FONTDATA_FT2_20101119_H__
#define __GUI_FONTDATA_FT2_20101119_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guifontdata.h>


//============================================================================//
// declare
//============================================================================// 


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	/**
	* @class IGUIFont_ft2
	* @brief use libfreetype2 as font engine,
	* support all font file type that libfreetye2 supports
	*/
	class GUIEXPORT CGUIFontData_ft2 : public CGUIFontData
	{
	protected:
		friend class IGUIFont_ft2;

	public:

	};

}//namespace guiex

#endif //__GUI_FONTDATA_FT2_20101119_H__
