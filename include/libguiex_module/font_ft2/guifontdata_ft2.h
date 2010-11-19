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
	class GUIEXPORT CGUIFontData_ft2 : public CGUIFontData
	{
	public:
		virtual ~CGUIFontData_ft2();

	protected:
		friend class IGUIFont_ft2;
		CGUIFontData_ft2( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex );
	
		virtual int32 DoLoad() const;
		virtual void DoUnload();

	protected:
		CGUIString m_strPath;
	};

}//namespace guiex

#endif //__GUI_FONTDATA_FT2_20101119_H__
