/** 
* @file guifontdata_dummy.h
* @brief use freetype2 as font engine
* @author ken
* @date 2011-11-19
*/

#ifndef __GUI_FONTDATA_DUMMY_20101119_H__
#define __GUI_FONTDATA_DUMMY_20101119_H__

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
	class GUIEXPORT CGUIFontData_dummy : public CGUIFontData
	{
	public:

	public:
		virtual ~CGUIFontData_dummy();

	protected:
		friend class IGUIFont_dummy;
		CGUIFontData_dummy( const CGUIString& rName, const CGUIString& rSceneName, const CGUIString& rPath, uint32 nFontID );
	
		virtual int32 DoLoad();
		virtual void DoUnload();

	public:

	};

}//namespace guiex

#endif //__GUI_FONTDATA_DUMMY_20101119_H__
