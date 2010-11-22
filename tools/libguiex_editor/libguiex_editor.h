/** 
 * @file libguiex_editor.h
 * @brief 
 * @author Lou Guoliang (louguoliang@gmail.com)
 * @date 2009-10-16
 */

#ifndef __LIBGUIEX_EDITOR_20091016_H__
#define __LIBGUIEX_EDITOR_20091016_H__


//============================================================================//
// include
//============================================================================//
// Disable the warning that the pack size is changed in this header.
#pragma warning(disable:4103)
#pragma warning(disable:4099)
#pragma pack(push,8)

//wxwidgets
#include "wx/wxprec.h"
#include "wx/glcanvas.h"
#include "wx/artprov.h"
#include "wx/treectrl.h"
#include "wx/tooltip.h"
#include "wx/filename.h"
#include "wx/aui/aui.h"
#include "wx/treebase.h"
#include "wx/wizard.h"
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/propdev.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>
#include "wx/app.h"
#include "wx/choicdlg.h"
#include "wx/colordlg.h"
//scinilla
#include <scintilla.h>
#include <Platform.h>
#include "ILexer.h"
#include <LexAccessor.h>
#include <Accessor.h>
#include <PropSetSimple.h>
#include <SciLexer.h>


#pragma pack(pop)


//libguiex
#include <libguiex_core/guiex.h>
#include <libguiex_widget\guiwgt.h>

//libguiex module
#include <libguiex_module\render_opengl\guirender_opengl.h>
#include <libguiex_module\imageloader_devil\guiimageloader_devil.h>
#include <libguiex_module\imageloader_tga\guiimageloader_tga.h>
#include <libguiex_module\keyboard_winapi\guikeyboard_winapi.h>
#include <libguiex_module\mouse_winapi\guimouse_winapi.h>
#include <libguiex_module\font_ft2\guifont_ft2.h>
#include <libguiex_module\font_dummy\guifont_dummy.h>
#include <libguiex_module\filesys_stdio\guifilesys_stdio.h>
#include <libguiex_module\configfile_tinyxml\guiconfigfile_tinyxml.h>
#include <libguiex_module\script_lua\guiscript_lua.h>
#include <libguiex_module\ime_winapi\guiime_winapi.h>
#include <libguiex_module\stringconv_winapi\guistringconv_winapi.h>



#include "propertyconfigmgr.h"
#include "propertysheetfunc.h"

#include "toolcache.h"

#include "windowbox.h"
#include "savefilebase.h"

#include "commandmanager.h"
#include "command.h"

#include "wxtexteditor.h"
#include "wxmainapp.h"
#include "wxmainframe.h"
#include "wxeditorcanvas.h"
#include "wxwidgetpropertysheet.h"
#include "wxoutputpanel.h"
#include "wxwizardcreatewidget.h"
#include "wxsavefiledlg.h"



//============================================================================//
// declare
//============================================================================//
#define STRING_M2W(value)	wxConvUTF8.cMB2WC((value).c_str())




//============================================================================//
// class
//============================================================================//




#endif	//__LIBGUIEX_EDITOR_20091016_H__

