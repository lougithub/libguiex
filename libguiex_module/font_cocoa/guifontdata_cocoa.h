/** 
* @file guifontdata_cocoa.h
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/

#ifndef __GUI_FONTDATA_COCOA_20101119_H__
#define __GUI_FONTDATA_COCOA_20101119_H__

//============================================================================//
// include
//============================================================================// 
#include <libguiex_core/guifontdata.h>
#include <libguiex_core/guirect.h>
#include <libguiex_core/guivector2.h>

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
}

//============================================================================//
// class
//============================================================================// 

namespace guiex
{
	class  CGUICharData_cocoa
	{
	public:
		CGUICharData_cocoa();
		~CGUICharData_cocoa();
		
	public:
		CGUITexture* m_pTexture;
		CGUIRect m_aUV;
		
		uint32 m_nGlyphIdx; //index of glyph
		int32 m_nBitmapWidth; //size of bitmap
		int32 m_nBitmapHeight; //size of bitmap
		int32 m_nLeftBearing; //left bearing of font
		int32 m_nBottomBearing; //bottom bearing of font
		int32 m_nTopBearing; //top bearing of font
		CGUISize m_aSize; //size of font
	};	
	
	class CGUICharsData_cocoa
	{
	public:
		CGUICharsData_cocoa();
		~CGUICharsData_cocoa();
		
	public:
		typedef	std::vector<CGUITexture*> TVecTexture;
		TVecTexture m_vecTexture;
		uint32 m_nX,m_nY; //start position of current free texture area
		
		typedef std::map<wchar_t, CGUICharData_cocoa*> TMapCharData;
		TMapCharData m_mapCharsData;
	};
	
	class GUIEXPORT CGUIFontData_cocoa : public CGUIFontData
	{
		
	public:
		virtual ~CGUIFontData_cocoa();

	protected:
		friend class IGUIFont_cocoa;
		CGUIFontData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex );
	
		virtual int32 DoLoad() const;
		virtual void DoUnload();

	public:
		CGUIString m_strPath;		// font file path
		mutable FT_Face m_aFtFace;
		typedef std::map<uint32, CGUICharsData_cocoa*> TMapSizeChars;
		TMapSizeChars m_mapSizeChars;

	};

}//namespace guiex

#endif //__GUI_FONTDATA_COCOA_20101119_H__
