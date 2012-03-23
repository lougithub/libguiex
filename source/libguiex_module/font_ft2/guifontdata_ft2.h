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
#include <libguiex_core/guirect.h>
#include <libguiex_core/guivector2.h>
#include <libguiex_core/guiintvector2.h>

#include <vector>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

//============================================================================//
// declare
//============================================================================// 
namespace guiex
{
	class CGUITexture;
	class CGUIFontFace_ft2;
}

//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	struct SCharData_ft2
	{
		CGUITexture* m_pTexture;
		CGUIRect m_aUV;

		uint32 m_nGlyphIdx; //index of glyph
		real m_fBitmapWidth; //size of bitmap
		real m_fBitmapHeight; //size of bitmap
		real m_fBearingX; //left bearing of font
		real m_fBearingY; //top bearing of font
		CGUISize m_aSize; //size of font

		SCharData_ft2()
			:m_pTexture(NULL)
			,m_fBearingX(0)
			,m_nGlyphIdx(0)
			,m_fBitmapWidth(0)
			,m_fBitmapHeight(0)
			,m_fBearingY(0)
		{
		}
	};	

	class GUIEXPORT CGUIFontData_ft2 : public CGUIFontData
	{
	public:
		virtual ~CGUIFontData_ft2();

		SCharData_ft2* GetCharData( wchar charCode );

	protected:
		friend class IGUIFont_ft2;
		CGUIFontData_ft2( 
			const CGUIString& rName, 
			const CGUIString& rSceneName, 
			const SFontInfo& rFontInfo,
			CGUIFontFace_ft2* pFontFace );

		virtual int32 DoLoad();
		virtual void DoUnload();

		SCharData_ft2* LoadCharData( wchar charCode );
		SCharData_ft2* LoadCharDataWithStroke( FT_Face pFontFace,uint32 uGlyphIdx, wchar charCode );
		SCharData_ft2* LoadCharDataWithoutStroke( FT_Face pFontFace, uint32 uGlyphIdx, wchar charCode, bool bUseMono );
		FT_Face GetFontFace();

	protected:
		CGUIFontFace_ft2* m_pFontFace; //font face

		typedef	std::vector<CGUITexture*> TVecTexture;
		TVecTexture m_vecTexture;
		uint32 m_uTexturePosX;
		uint32 m_uTexturePosY;
		uint32 m_uMaxHeight;

		typedef std::map<wchar, SCharData_ft2*> TMapCharData;
		TMapCharData m_mapCharsData;
	};

}//namespace guiex

#endif //__GUI_FONTDATA_FT2_20101119_H__
