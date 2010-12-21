/** 
* @file guiframeworkeditor.cpp
* @author ken
* @date 2010-12-21
*/


//============================================================================//
// include
//============================================================================//
#include "guiframeworkeditor.h"
#include <libguiex_module\render_opengl\guirender_opengl.h>
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

//============================================================================//
// function
//============================================================================//
//------------------------------------------------------------------------------
CGUIFrameworkEditor* CGUIFrameworkEditor::ms_pFramework = NULL;
//------------------------------------------------------------------------------
CGUIFrameworkEditor::CGUIFrameworkEditor( )
:CGUIFramework( )
{
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::RegisterInterfaces( )
{
	//register interface
	GUI_REGISTER_INTERFACE_LIB( IGUIImageLoader_tga);
	GUI_REGISTER_INTERFACE_LIB( IGUIFileSys_stdio);
	GUI_REGISTER_INTERFACE_LIB( IGUIMouse_winapi);
	GUI_REGISTER_INTERFACE_LIB( IGUIFont_ft2);
	GUI_REGISTER_INTERFACE_LIB( IGUIKeyboard_winapi);
	GUI_REGISTER_INTERFACE_LIB( IGUIConfigFile_tinyxml);
	GUI_REGISTER_INTERFACE_LIB( IGUIStringConv_winapi);
	GUI_REGISTER_INTERFACE_LIB_ARG( IGUIIme_winapi, ::GetForegroundWindow() );
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::RegisterOpenglInterface()
{
	GUI_REGISTER_INTERFACE_LIB( IGUIRender_opengl );
}
//------------------------------------------------------------------------------
void CGUIFrameworkEditor::UnregisterOpenglInterface()
{
	GUI_UNREGISTER_INTERFACE_LIB( IGUIRender_opengl);
}
//------------------------------------------------------------------------------
