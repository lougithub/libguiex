/** 
* @file toolsmisc.h
* @author ken
* @date 2011-09-08
*/


#ifndef __KEN_TOOLSMISC_20110908_H__
#define __KEN_TOOLSMISC_20110908_H__


//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include "libguiex_core/guiex.h"

//============================================================================//
// declare
//============================================================================//
extern wxString Gui2wxString( const guiex::CGUIString& rString );
extern guiex::CGUIString wx2GuiString( const wxString& rString );
wxImage* LoadwxImageByGuiImage( const guiex::CGUIImage* pGuiImage );
wxImage* LoadwxImageByFilename( const wxString& rFilename );

#endif //__KEN_TOOLSMISC_20110908_H__
