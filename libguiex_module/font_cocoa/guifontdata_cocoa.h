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
	class GUIEXPORT CGUIFontData_cocoa : public CGUIFontData
	{
	public:

	public:
		virtual ~CGUIFontData_cocoa();

	protected:
		friend class IGUIFont_cocoa;
		CGUIFontData_cocoa( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontIndex );
	
		virtual int32 DoLoad() const;
		virtual void DoUnload();

	public:

	};

}//namespace guiex

#endif //__GUI_FONTDATA_COCOA_20101119_H__
