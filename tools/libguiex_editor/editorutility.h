/** 
* @file editorutility.h
* @author ken
*/

#ifndef __KEN_EDITOR_UTILITY_20101217_H__
#define __KEN_EDITOR_UTILITY_20101217_H__


//============================================================================//
// include
//============================================================================// 
#include <wx/wxprec.h>
#include "libguiex_core/guiex.h"

//============================================================================//
// declare
//============================================================================//
using namespace guiex;
extern wxString Gui2wxString( const CGUIString& rString );
extern CGUIString wx2GuiString( const wxString& rString );
wxImage* LoadwxImageByGuiImage( const CGUIImage* pGuiImage );

#endif //__KEN_EDITOR_UTILITY_20101217_H__
