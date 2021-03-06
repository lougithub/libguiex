/** 
* @file guifontface_ft2.h
* @brief use freetype2 as font engine
* @author ken
* @date 2011-04-20
*/

#ifndef __GUI_FONTFACE_FT2_20110420_H__
#define __GUI_FONTFACE_FT2_20110420_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guireference.h>

#include <ft2build.h>
#include FT_FREETYPE_H


//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	class IGUIFont_ft2;

	/**
	* @class CGUIFontFace_ft2
	* @brief use libfreetype2 as font engine,
	*/
	class GUIEXPORT CGUIFontFace_ft2 : public CGUIReference
	{
	public:
		FT_Face GetFontFace();

		FT_Library& GetFTLibrary();


	protected:
		friend class IGUIFont_ft2;
		CGUIFontFace_ft2( IGUIFont_ft2* pInterfaceFont, const CGUIString& rFontPath );
		virtual ~CGUIFontFace_ft2();

		void LoadFont( const CGUIString& rFontPath );
		void UnloadFont();

	protected:
		FT_Face m_aFtFace;
		IGUIFont_ft2* m_pInterfaceFont;
	};
}//namespace guiex

#endif //__GUI_FONTFACE_FT2_20110420_H__
